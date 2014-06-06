#include "weighted_showdown_eval.hpp"

namespace freedom {

using namespace poker;

WeightedShowdownEval::WeightedShowdownEval(ECalc *_ecalc, int _samples)
    : ShowdownEval(_ecalc, _samples) {}

// BAD, errors happenb when someone uses this
WeightedShowdownEval::~WeightedShowdownEval() {}

double WeightedShowdownEval::simulate(const FContext &context) {
  Handlist::collection_t lists(0); // TODO size is known
  int index_bot = context.index_bot;
  int bot_result_index = -1;

  // get active players handlists
  for (unsigned i = 0; i < context.player.size(); ++i) {
    if (!context.player[i].is_inactive()) {
      // modify ranges according to actions
      if (index_bot == (int)i) {
        lists.push_back(context.player[i].handlist);
        // save relative bot index for retrival
        bot_result_index = lists.size() - 1;
      } else {
        string sequence = context.player[i].action_sequence.str();
        string key = std::to_string(i) + sequence;
        if (!is_cached(key)) {
          WeightedBucketHandlist *hl =
              static_cast<WeightedBucketHandlist *>(context.player[i].handlist);
          ActionSequence seq =
              context.player[i].action_sequence.subtract(hl->range_sequence);
          vector<double> weights = modify_range(hl->weights, seq);
          cache[key] = new WeightedBucketHandlist(*hl, weights);
        }
        lists.push_back(cache[key]);
      }
    }
  }

  ecalc::result_collection res;
  res = ecalc->evaluate(lists, context.config->board, vector<unsigned>(),
                        samples);
  ecalc::Result bot_result = res[bot_result_index];

  bb fixedreturn = get_fixed_win(context);
  bb notwinnable = get_not_winnable_pot(context);
  bb winnable_pot = context.pot - fixedreturn - notwinnable;

  double ev = bot_result.pwin_tie() * winnable_pot.getAsDouble();

  // subtract our investment to the pot
  bb player_investment = context.bot_player().total_investment();
  bb our_investment = player_investment - fixedreturn;
  ev -= our_investment.getAsDouble();

  return (1 - context.config->rake_factor) * ev;
}

vector<double>
WeightedShowdownEval::modify_range(const vector<double> &bucket_probabilities,
                                   const ActionSequence &seq) const {
  vector<double> new_weights = bucket_probabilities;

  // sum up weights
  unsigned sum = 0;
  for (unsigned i = 0; i < new_weights.size(); ++i)
    sum += new_weights[i];

  PhaseType::Enum curr_phase;
  vector<FActionSequence::LineAction> curr_line;
  for (int i = 0; i < 4; ++i) {
    curr_phase = static_cast<PhaseType::Enum>(i);
    // iterate over all actions in this phase
    curr_line = seq.sequence[curr_phase];
    if (curr_line.empty())
      continue;
    // loop over actions in phase
    for (FActionSequence::LineAction action : curr_line) {
      if (action.action.action == ActionType::Call) {
        // TODO make gauss and exp dist configurable from freedom_node
        apply_gauss(new_weights, (new_weights.size() / 2.0) - 1, sum / 2.0);
      }
      if (action.action.action == ActionType::Raise) {
        // TODO make gauss and exp dist configurable from freedom_node
        apply_exp(new_weights, 0.75, sum / 2.0);
      }
    }
  }

  return new_weights;
}

void WeightedShowdownEval::apply_exp(vector<double> &vec, const double &lambda,
                                     const double &multiplicator) const {
  ExponentialDistribution exp_dist(lambda);
  for (unsigned i = 0; i < vec.size(); ++i)
    vec[i] += exp_dist(i) * multiplicator;
}

void WeightedShowdownEval::apply_gauss(vector<double> &vec, const double &mean,
                                       const double &std_dev) const {
  GaussianDistribution gauss_dist(mean, std_dev);
  for (unsigned i = 0; i < vec.size(); ++i)
    vec[i] += gauss_dist(i);
}
}
