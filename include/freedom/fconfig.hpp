/*
 * File:   fconfig.h
 * Author: batman
 *
 * Created on August 6, 2013, 12:37 PM
 */

#ifndef FCONFIG_H
#define FCONFIG_H

#include "schema_exception.hpp"
#include <poker/pokerdefs.hpp>
#include <ecalc/ecalc.hpp>
#include <mcts/iselection_strategy.hpp>
#include <mcts/isimulation_strategy.hpp>
#include <mcts/ibackpropagation_strategy.hpp>
#include <rapidjson/document.h>
#include <strategy_map.hpp>
#include <unordered_map>
#include "model.hpp"

namespace Freedom5 {

using rapidjson::Value;
using ecalc::ECalc;
using mcts::ISelectionStrategy;
using mcts::ISimulationStrategy;
using mcts::IBackpropagationStrategy;
using namespace poker;

class FConfig {
  typedef std::mt19937 generator_t;
  typedef std::unordered_map<std::string, Model *> model_map;
  typedef typename ISelectionStrategy<FContext, FConfig>::sstrategy_t
  sstrategy_t;
  typedef typename ISimulationStrategy<FContext>::simstrategy_t simstrategy_t;

public:
  generator_t *nb_gen_;
  double time_s;
  int ecalc_nb_samples;
  bool dump_tree;
  model_map models;

  ECalc *ecalc;
  simstrategy_t *simulation_strat;
  sstrategy_t *move_selection_strat_;
  sstrategy_t *decision_selection_strat;
  sstrategy_t *opponent_selection_strat;
  IBackpropagationStrategy *decision_backprop_strat;
  IBackpropagationStrategy *opponent_backprop_strat;

  FConfig(const Value &data, ECalc *_ecalc, bool seed = true);
  FConfig(sstrategy_t *_move_select_strat, bool seed = true,
          bool _dump_tree = false)
      : move_selection_strat_(_move_select_strat), dump_tree(_dump_tree),
        nb_gen_(new generator_t(0)) {} // TODO seed generator

  // TODO clone strategies here. so constructor can take care of
  // deletion without external segfaults
  FConfig(double _time_s, int _ecalc_nb_samples, ECalc *_ecalc,
          simstrategy_t *_simulation_strat,
          IBackpropagationStrategy *_decision_backprop_strat,
          IBackpropagationStrategy *_opponent_backprop_strat,
          sstrategy_t *_decision_selection_strat,
          sstrategy_t *_opponent_selection_strat,
          sstrategy_t *_move_select_strat, bool seed = true,
          bool _dump_tree = false)
      : move_selection_strat_(_move_select_strat),
      //TODO seed gen
        nb_gen_(new generator_t(0)), time_s(_time_s),
        ecalc_nb_samples(_ecalc_nb_samples), ecalc(_ecalc),
        simulation_strat(_simulation_strat),
        decision_backprop_strat(_decision_backprop_strat),
        opponent_backprop_strat(_opponent_backprop_strat),
        decision_selection_strat(_decision_selection_strat),
        opponent_selection_strat(_opponent_selection_strat),
        dump_tree(_dump_tree) {}

  ~FConfig();

  Model *model(string name);
  void readModels(const Value &data);

  generator_t *nb_gen() { return nb_gen_; }
  sstrategy_t *move_selection_strat() { return move_selection_strat_; }
};
};

#endif /* CONFIG_H */

