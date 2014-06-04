#include "range_predictor.hpp"
#include "phase_histogramm.hpp"
#include <thread>
#include <sstream>
#include <iostream>
#include <bitset>
#include <ecalc/result.hpp>
#include <ecalc/single_handlist.hpp>
#include <ecalc/random_handlist.hpp>

namespace freedom {

using std::thread;

RangePredictor::RangePredictor(ECalc *_calc, int _nb_samples,
                               Threshold _thresholds, vector<int> _nb_buckets,
                               int _threshold_min_hands, double _zerop_rc,
                               bool _use_cache)
    : nb_samples(_nb_samples), ecalc(_calc), ecalc2(_calc),
      threshold(_thresholds), nb_buckets(_nb_buckets),
      threshold_min_hands(_threshold_min_hands), zerop_rc(_zerop_rc),
      use_cache(_use_cache) {}

vector<BucketHand> RangePredictor::top_n_hands(BucketCollection &buckets,
                                               int nb_hands) {
  int end = buckets.get_bucket_index_including_nb_hands(nb_hands);
  return buckets.hand_bucket_range(end, 0);
}

// TODO ein bitset als id wäre sehr viel effizienter
string RangePredictor::generate_cache_id(vector<unsigned> board,
                                         vector<unsigned> deadcards) {
  // TODO "for" makes unnecessary copies
  std::stringstream ss;
  vector<Card> cards;
  for (unsigned c : board)
    cards.push_back(Card(c));
  for (unsigned c : deadcards)
    cards.push_back(Card(c));
  std::sort(cards.begin(), cards.end());
  for (auto c : cards)
    ss << c.str();
  return ss.str();
}

Hand RangePredictor::get_best_hand(vector<unsigned> board,
                                   vector<unsigned> deadcards) {
  string id = build_cache(board, deadcards);
  vector<BucketHand> hands = cache[id];
  return static_cast<Hand>(hands[0]);
}

double RangePredictor::calculate_best_hand_percentage(double p_fold) const {
  double factor = 1 / 3.0;

  // player folds less than 33%
  if (p_fold < factor) {
    return 0;
  }

  // player folds more than 33% and less than 66%
  if (p_fold >= factor && p_fold < (2 * factor)) {
    double ratio = ((p_fold - factor) / factor);
    return factor * (ratio * ratio);
  }

  // player folds more than 66%
  if (p_fold > factor) {
    return p_fold - factor;
  }

  // for safety, but should never be reached
  return 0;
}

string RangePredictor::build_cache(vector<unsigned> board,
                                   vector<unsigned> deadcards) {
  string id = generate_cache_id(board, deadcards);
  if (cache.find(id) == cache.end()) {
    vector<unsigned> cards(board);
    cards.insert(cards.end(), deadcards.begin(), deadcards.end());
    cache[id] = hand_combinations(cards);
    calculate_handstrengths(cache[id], board, deadcards);

    // best equities are at the front ( index 0 )
    std::sort(cache[id].begin(), cache[id].end());
    std::reverse(cache[id].begin(), cache[id].end());
  }
  return id;
}

vector<BucketHand>
RangePredictor::hand_combinations(vector<unsigned> deadcards) {
  std::bitset<52> deadmap(0);
  for (int card : deadcards)
    deadmap[card - 1] = 1;

  int c0, c1;
  vector<BucketHand> hands;
  for (c0 = 0; c0 < 51; c0++)
    for (c1 = c0 + 1; c1 < 52; c1++)
      if (!deadmap[c0] && !deadmap[c1])
        hands.push_back(BucketHand(c0 + 1, c1 + 1, 0));
  return hands;
}

double RangePredictor::equity_first(vector<Handlist *> lists,
                                    vector<unsigned> board,
                                    vector<unsigned> deadcards, ECalc *c) {
  ecalc::result_collection results;
  results = c->evaluate(lists, board, deadcards, nb_samples);
  return results[0].pwin_tie();
}

void RangePredictor::calculate_subset(vector<BucketHand> &hands,
                                      vector<unsigned> board,
                                      vector<unsigned> deadcards, ECalc *c) {
  if (hands.empty())
    return;

  // initialize single handlist with first hand, for init reasons
  vector<Handlist *> lists({new ecalc::SingleHandlist(hands[0]),
                            // all samples against random
                            new ecalc::RandomHandlist()});

  for (unsigned i = 0; i < hands.size(); ++i) {
    static_cast<ecalc::SingleHandlist *>(lists[0])->set_hand(hands[i]);
    hands[i].equity = equity_first(lists, board, deadcards, c);
  }

  // free handlists
  delete lists[0], lists[1];
}

void RangePredictor::calculate_handstrengths(vector<BucketHand> &hands,
                                             vector<unsigned> board,
                                             vector<unsigned> deadcards) {

  std::size_t const half_size = hands.size() / 2;
  vector<BucketHand> b1(hands.begin(), hands.begin() + half_size);
  vector<BucketHand> b2(hands.begin() + half_size, hands.end());

  std::thread a(&RangePredictor::calculate_subset, this, std::ref(b1), board,
                deadcards, ecalc);
  std::thread b(&RangePredictor::calculate_subset, this, std::ref(b2), board,
                deadcards, ecalc2);

  a.join();
  b.join();

  vector<BucketHand> b3(b1);
  b3.insert(b3.end(), b2.begin(), b2.end());
  hands = b3;
}

bool RangePredictor::is_raise_or_bet_or_allin(const Action &action) const {
  return action.is_raise() || action.is_bet() || action.is_allin();
}

int RangePredictor::calculate_upper_bound(Action action, PhaseType::Enum phase,
                                          int betting_round, Model *model,
                                          BucketCollection &collection) {
  // top bucket for raises and alike aswell as weak actions
  if (is_raise_or_bet_or_allin(action) ||
      is_weak_call_or_raise(action, !(phase == PhaseType::Preflop)))
    return 0;

  if (action.is_call()) {
    // get raise percentage to skip it
    double p_raise =
        model->get_action_probability(action, phase, betting_round);
    int skiphands = (int)ceil(p_raise * collection.count_hands());
    return collection.get_bucket_index_including_nb_hands(skiphands);
  }

  // fallthrough for fold and check
  return 0;
}

int RangePredictor::calculate_lower_bound(Action action, PhaseType::Enum phase,
                                          int betting_round, Model *model,
                                          BucketCollection &collection,
                                          int upper_bound) {
  if (is_weak_call_or_raise(action, !(phase == PhaseType::Preflop)))
    return collection.nb_buckets - 1;

  if (is_raise_or_bet_or_allin(action)) {
    double p_raise =
        model->get_action_probability(action, phase, betting_round);
    // fallback when models probability is zero
    p_raise = (p_raise > 1e-10) ? p_raise : zerop_rc;
    int includehands = (int)ceil(p_raise * collection.count_hands());
    return collection.get_bucket_index_including_nb_hands(includehands,
                                                          upper_bound);
  }

  if (action.is_call()) {
    double p_call = model->get_action_probability(action, phase, betting_round);
    // fallback when model's probability is zero
    p_call = (p_call > 1e-10) ? p_call : zerop_rc;
    int includehands = (int)ceil(p_call * collection.count_hands());
    return collection.get_bucket_index_including_nb_hands(includehands,
                                                          upper_bound);
  }

  // fallthrough for fold check, lowest bucket
  return collection.nb_buckets - 1;
}

bool RangePredictor::is_weak_call_or_raise(Action action, bool is_postflop) {
  if (is_raise_or_bet_or_allin(action) || action.is_call())
    if (action.amount <=
        bb((is_postflop ? threshold.postflop : threshold.preflop)))
      return true;
  return false;
}

vector<unsigned> RangePredictor::board_by_phase(vector<unsigned> complete_board,
                                                PhaseType::Enum phase) {
  vector<unsigned> board;
  switch (phase) {
  case PhaseType::Flop:
    if (complete_board.size() < 3)
      throw std::logic_error("Not enough cards for requested Phase");
    board.assign(complete_board.begin(), complete_board.begin() + 3);
    break;
  case PhaseType::Turn:
    if (complete_board.size() < 4)
      throw std::logic_error("Not enough cards for requested Phase");
    // TODO first call obsolete? gets oberwridden by next statement, test this
    board.assign(complete_board.begin(), complete_board.begin() + 3);
    board.assign(complete_board.begin(), complete_board.begin() + 4);
    break;
  case PhaseType::River:
    if (complete_board.size() < 5)
      throw std::logic_error("Not enough cards for requested Phase");
    // TODO first call obsolete? gets oberwridden by next statement, test this
    board.assign(complete_board.begin(), complete_board.begin() + 3);
    board.assign(complete_board.begin(), complete_board.begin() + 5);
    break;
  }
  return board;
}

vector<unsigned> RangePredictor::dead_by_phase(vector<unsigned> complete_board,
                                               vector<unsigned> deadcards,
                                               PhaseType::Enum phase) {
  vector<unsigned> dead = deadcards;
  switch (phase) {
  case PhaseType::Preflop:
    dead.insert(dead.end(), complete_board.begin(), complete_board.end());
    break;
  case PhaseType::Flop:
    dead.insert(dead.end(), complete_board.begin() + 3, complete_board.end());
    break;
  case PhaseType::Turn:
    dead.insert(dead.end(), complete_board.begin() + 4, complete_board.end());
    break;
  }
  return dead;
}

// possibly slow ( have to see how it pans out ) TODO benchmark and make more
// efficent
vector<BucketHand>
RangePredictor::update_equities(vector<BucketHand> &overwrite,
                                vector<BucketHand> &source) const {
  vector<BucketHand> out;
  for (unsigned o = 0; o < overwrite.size(); ++o)
    for (unsigned s = 0; s < source.size(); ++s)
      if (static_cast<Hand>(overwrite[o]) == static_cast<Hand>(source[s]))
        out.push_back(source[s]);
  return out;
}
};
