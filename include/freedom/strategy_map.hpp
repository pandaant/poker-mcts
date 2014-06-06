#ifndef STRATEGY_MAP_H
#define STRATEGY_MAP_H

#include <unordered_map>
#include <string>
#include <mcts/max_value_selector.hpp>
#include <mcts/min_sample_selector.hpp>
#include <mcts/sampling_selector.hpp>
#include <mcts/sampling_to_function_selector.hpp>
#include <mcts/uct_selector.hpp>
#include <mcts/avg_backpropagation_strategy.hpp>
#include "model_selector.hpp"
#include "betamt_ev_ratio_selector.hpp"
#include "ev_pot_ratio_selector.hpp"
#include "final_move_selector.hpp"

namespace freedom {
namespace StrategyMap {

using std::unordered_map;
using std::string;

// ----------------------------------------------------------------------
/// @brief   converts a string to a simulation strategy.
///
/// @tparam Context a state context
/// @tparam Config a config object
/// @param strategy strategy to look up
///
/// @return strategy if found. if not found, exception is thrown.
// ----------------------------------------------------------------------
template <typename Context, typename Config>
static ISelectionStrategy<Context, Config> *
lookup_selection_strat(const string &strategy) {
  if (strategy == "sampling")
    return new SamplingSelector<Context, Config>();
  if (strategy == "max_value")
    return new MaxValueSelector<Context, Config>();
  if (strategy == "min_sample")
    return new MinSampleSelector<Context, Config>();
  if (strategy == "sample_to_uct")
    return new SamplingToFunctionSelector<Context, Config>(
        20000, new UCTSelector<Context, Config>(25));
  if (strategy == "model_selector")
    return new ModelSelector<Config>();
  if (strategy == "ev_amt_ratio_selector")
    return new EVPotRatioSelector();
  if (strategy == "betamt_ev_ratio_selector")
    return new BetamtEVRatioSelector(1);
  if (strategy == "final_move_selector")
    return new FinalMoveSelector(1.0, 3, 1.5);
  throw std::runtime_error("requested selection strategy not found.");
}

// ----------------------------------------------------------------------
/// @brief   converts a string to backpropagation strategy.
///
/// @param strategy strategy to look up
///
/// @return strategy if found. if not found, exception is thrown.
// ----------------------------------------------------------------------
static IBackpropagationStrategy *lookup_backprop_strat(const string &strategy) {
  if (strategy == "sample_weighted")
    return new AvgBackpropagationStrategy();
  throw std::runtime_error("requested backprop strategy not found.");
}
}
}

#endif
