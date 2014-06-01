/*
 * File:   range_predictor.h
 * Author: batman
 *
 * Created on August 13, 2013, 11:48 PM
 */

#ifndef RANGE_PREDICTOR_H
#define RANGE_PREDICTOR_H

#include <vector>
#include <ecalc/ecalc.hpp>
#include <ecalc/handlist.hpp>
#include <poker/action.hpp>
#include <poker/action_sequence.hpp>
#include <unordered_map>
#include "model.hpp"
#include "bucket_hand.hpp"
#include "bucket_collection.hpp"
#include "weighted_bucket_handlist.hpp"

namespace Freedom5 {
using std::vector;
using ecalc::ECalc;
using ecalc::Handlist;
using poker::Hand;
using poker::Action;
using poker::ActionSequence;

struct Threshold {
  double preflop, postflop;

  Threshold(double prf, double pof) : preflop(prf), postflop(pof) {}
};

//TODO make range predictor base class an derive from there. at the moment
// ehs2 handpredictor overwrites equity_first and because of this
// in this class the function has to be made virtual
/**
 * Big change on rangepredictor.
 * if a board is given, the cards will be
 * used as deadcards. if 4 boardcards are given
 * and preflop handrange is calculated, the 1 turn
 * card is added to the deadcards.
 **/
class RangePredictor {
protected:
  ECalc *ecalc;
  // is a copy of ecalc for threading
  ECalc *ecalc2;
  int nb_samples;
  Threshold threshold;
  vector<int> nb_buckets;
  int threshold_min_hands;
  // if an action has zero % of occuring,
  // this percentage is used instead
  // for raise call actions.
  double zerop_rc;
  bool use_cache;

public:
  //TODO clear when max size reached to keep cache from growing
  // to big
  // TODO i could use a ordnered map here, because faster lookup and stuff
  // cache table
  std::unordered_map<string, vector<BucketHand> > cache;

  /**
   * Constructor
   * @param nb_samples   nb samples for equity calculations
   * @param thresholds   weak actions amounts for preflop/postflop
   * @param nb_buckets   vector with 4 elements. each one is the number of
   * @param thres. nb h. smallest number of hands range can have
   * @param zerop_rc     if zero percent for a model action use this when call
   * or raise percentage
   * buckets to use in that round (0=preflop, 3=river)
   */
  RangePredictor(ECalc *_calc, int _nb_samples,
                 Threshold _thresholds = Threshold(2, 2),
                 vector<int> _nb_buckets = vector<int>(4, 20),
                 int _threshold_min_hands = 5,
                 double _zerop_rc = 0.20,
                 bool _use_cache = true);

  /**
   * Generates a range for a given action/model combination
   * @param actions
   * @param model
   * @param board
   * @param dead
   * @return
   */
  template <class Bucketizer>
  vector<Hand> predict_range(ActionSequence actions, Model *model,
                             vector<unsigned> board, vector<unsigned> dead) {

    vector<BucketHand> collection = _predict_range<Bucketizer>(actions,model,board,dead);
    // ugly conversion
    vector<Hand> range;
    for (auto bhand : collection)
      range.push_back(static_cast<Hand>(bhand));
    return range;
  }

  template <class Bucketizer>
  vector<BucketHand> _predict_range(ActionSequence actions, Model *model,
                             vector<unsigned> board, vector<unsigned> dead) { // TODO use ecalcdefs here?
    vector<Hand> current_range;

    // get initial
    vector<BucketHand> temp_collection;
    vector<BucketHand> collection = cache[build_cache(
        vector<unsigned>(), dead_by_phase(board, dead, PhaseType::Preflop))];

    PhaseType::Enum curr_phase;
    vector<ActionSequence::LineAction> curr_line;
    // BalancedBucketizer buckbuck;
    Bucketizer buckbuck;

    // temp identifier
    string pid;

    // iterate over all phases
    for (int i = 0; i < 4; ++i) {
      curr_phase = static_cast<PhaseType::Enum>(i);
      // iterate over all actions in this phase
      curr_line = actions.sequence[curr_phase];
      if (curr_line.empty())
        continue;
      // TODO also check that only actions are processed in phases where
      // we have enough boardcards. this is a prevention ... i dont know 
      // .. maybe its ok like this. if it happens, would mean that the
      // supplied history is wrong. so we better fold because of an error
      // than calculate wrong results because of wrong input

      // build a cache for the current phase
      pid = build_cache(board_by_phase(board, curr_phase),
                        dead_by_phase(board, dead, curr_phase));

      // apply new equities to range
      collection = update_equities(collection, cache[pid]); //TODO delete, is obsolete

      // loop over actions in phase
      for (ActionSequence::LineAction action : curr_line) {
        int upper, lower;

        // map hands to bucket
        BucketCollection buckets =
            buckbuck.map_hands(nb_buckets[i], collection);
        // calc bounds
        upper = calculate_upper_bound(action.action, curr_phase, action.betting_round,  model, buckets);
        lower =
            calculate_lower_bound(action.action, curr_phase, action.betting_round, model, buckets, upper);

        // new range
        temp_collection = buckets.hand_bucket_range(lower, upper);

        // check if not to small
        collection = (temp_collection.size() < threshold_min_hands)
                         ? top_n_hands(buckets, threshold_min_hands)
                         : temp_collection;
      }
    }

    // remove entries from cache if we 
    // dont wanna use it
    if (!use_cache)
      cache.clear();

    return collection;
  }

  /**
   * returns the hand with the best equity for a given board
   * and deadcards.
   */
  Hand get_best_hand(vector<unsigned> board, vector<unsigned> deadcards);

  /**
   * returns the percentage the best hand possible will be sampled
   * by the showdown eval.
   * Formula from:
   * An Exploitative Monte-Carlo Poker Agent
   * Technical Report TUD-KE-2009-2
   * Chapter 4.4.2
   *
   * @param p_fold percentage of hands folded in context
   * @param board,deadcards vectors of cards
   */
  double calculate_best_hand_percentage(double p_fold) const;

  /**
   * generates an id to identify board/deadcard combination
   * @param board
   * @param deadcards
   * @return
   */
  string generate_cache_id(vector<unsigned> board, vector<unsigned> deadcards);

  /**
   * looks for the entry. if it cant find it, it will be generated and saved
   * @param board
   * @param deadcards
   * @return the uniqe identdifier for this cache entry as created by
   * generate_cache_id
   */
  string build_cache(vector<unsigned> board, vector<unsigned> deadcards);

  /**
   * calculates all possible hands
   * @param deadcards
   * @return
   */
  vector<BucketHand> hand_combinations(vector<unsigned> deadcards);

  /**
   * calculates equity (pwin+ptie) for the first handlist
   * @param lists
   * @param board
   * @param deadcards
   * @return
   */
  virtual double equity_first(vector<Handlist *> lists, vector<unsigned> board,
                      vector<unsigned> deadcards, ECalc *c);

  /**
   * calculates equities for a range of hands in 2 threads of calculate_subset
   * @param hands
   * @param board
   * @param deadcards
   */
  void calculate_handstrengths(vector<BucketHand> &hands, vector<unsigned> board,
                               vector<unsigned> deadcards);

  /**
   * threaded calculation of range
   * @param hands
   * @param board
   * @param deadcards
   * @param c
   */
  void calculate_subset(vector<BucketHand> &hands, vector<unsigned> board,
                        vector<unsigned> deadcards, ECalc *c);

  /**
   * updates collection @overwrite
   * loops through every hand and performs following actions:
   *    - if hand is present in source, copy the equity from source to overwrite
   *    - if hand is not present in source, delete it from collection
   * @param overwrite   collection to be modified
   * @param source      reference collection
   */
  //TODO depreciated, not needed anymore because now it is impossible 
  // to carry hands that cant be because they use a board or a deadcard
  vector<BucketHand> update_equities(vector<BucketHand> &overwrite,
                                     vector<BucketHand> &source) const;

  /**
   * the upper_bound is the index of the first bucket to get hands from.
   * The BucketCollection stores strong hands at low indexes and weaker in 
   * ascending order. because of this the upper_bound is always >= lower_bound.
   * contrary to intuition.
   */
  int calculate_upper_bound(Action action, PhaseType::Enum phase, int betting_round, Model *model,
                            BucketCollection &collection);
  int calculate_lower_bound(Action action, PhaseType::Enum phase, int betting_round, Model *model,
                            BucketCollection &collection, int upper_bound);

  bool is_weak_call_or_raise(Action action, bool is_postflop);

  vector<unsigned> board_by_phase(vector<unsigned> complete_board, PhaseType::Enum phase);

  /**
   * returns deadcards concatenated with boardcards.
   * but only that boardcards that are not used in the current phase
   * (because they are from later phases). but we already know them,
   * so we use them.
   **/
  vector<unsigned> dead_by_phase(vector<unsigned> complete_board, vector<unsigned> dead_cards,PhaseType::Enum phase);

  /**
   * returns top nb_hands of hands rounded up to the buckets that contain them
   * EXAMPLE: if each bucket has 10 hands and you want the top 5 hands, 10 hands
   * will
   * be returned, because the whole bucket is returned.
   */
  vector<BucketHand> top_n_hands(BucketCollection &buckets, int nb_hands);

  bool is_raise_or_bet_or_allin(const Action &action) const;
};
};

#endif /* RANGE_PREDICTOR_H */

