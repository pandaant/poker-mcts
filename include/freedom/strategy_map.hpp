/*
 * File:   strategy_map.h
 * Author: batman
 *
 * Created on August 18, 2013, 5:31 PM
 */

#ifndef STRATEGY_MAP_H
#define STRATEGY_MAP_H

#include <unordered_map>
#include <string>
#include <mcts/max_value_selector.hpp>
#include <mcts/min_sample_selector.hpp>
#include <mcts/sampling_selector.hpp>
#include <mcts/sampling_to_function_selector.hpp>
#include <ev_pot_ratio_selector.hpp>
#include <mcts/uct_selector.hpp>
#include <mcts/avg_backpropagation_strategy.hpp>
#include <betamt_ev_ratio_selector.hpp>
#include <final_move_selector.hpp>
#include <model_selector.hpp>

namespace freedom {
namespace StrategyMap {

using std::unordered_map;
using std::string;
using namespace mcts;

template<typename Context, typename Config>
static ISelectionStrategy<Context, Config> *lookup_selection_strat(string strategy) {
  if (strategy == "sampling")
    return new SamplingSelector<Context, Config>();
  if (strategy == "max_value")
    return new MaxValueSelector<Context, Config>();
  if (strategy == "min_sample")
    return new MinSampleSelector<Context, Config>();
  if (strategy == "sample_to_uct")
    return new SamplingToFunctionSelector<Context, Config>(20000, new UCTSelector<Context, Config>(25));
  if (strategy == "model_selector")
    return new ModelSelector<Context, Config>();
  if (strategy == "ev_amt_ratio_selector")
    return new EVPotRatioSelector<Context, Config>();
  if (strategy == "betamt_ev_ratio_selector")
    return new BetamtEVRatioSelector<Context, Config>(1);
  if (strategy == "final_move_selector")
    return new FinalMoveSelector<Context, Config>(1.0,3,1.5);
  throw std::runtime_error("requested selection strategy not found.");
}

static IBackpropagationStrategy *lookup_backprop_strat(string strategy) {
  /*if (strategy == "robust_max")
      return new RobustMaxBackpropagationStrategy();*/
  if (strategy == "sample_weighted")
    return new AvgBackpropagationStrategy();
  throw std::runtime_error("requested backprop strategy not found.");
}
};
};

#endif /* STRATEGY_MAP_H */

