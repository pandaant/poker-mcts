#ifndef FCONFIG_H
#define FCONFIG_H

#include "schema_exception.hpp"
#include <poker/pokerdefs.hpp>
#include <ecalc/ecalc.hpp>
#include <mcts/iselection_strategy.hpp>
#include <mcts/isimulation_strategy.hpp>
#include <mcts/ibackpropagation_strategy.hpp>
#include <rapidjson/document.h>
#include <unordered_map>
#include "strategy_map.hpp"
#include "imodel.hpp"

namespace freedom {

using ecalc::ECalc;
using rapidjson::Value;
using mcts::ISelectionStrategy;
using mcts::ISimulationStrategy;
using mcts::IBackpropagationStrategy;
using namespace poker;

class FConfig {
  typedef std::mt19937 generator_t;
  typedef std::unordered_map<std::string, IModel *> model_map;
  typedef typename ISimulationStrategy<FContext>::simstrategy_t simstrategy_t;
  typedef typename ISelectionStrategy<FContext, FConfig>::sstrategy_t
  sstrategy_t;

public:
  generator_t *nb_gen_;
  double time_s;
  int ecalc_nb_samples;
  model_map models;

  ECalc *ecalc;
  simstrategy_t *simulation_strat;
  sstrategy_t *move_selection_strat;
  sstrategy_t *decision_selection_strat;
  sstrategy_t *opponent_selection_strat;
  IBackpropagationStrategy *decision_backprop_strat;
  IBackpropagationStrategy *opponent_backprop_strat;

  FConfig(const Value &data, ECalc *ecalc_, const bool &seed = true);

  // TODO clone strategies here. so constructor can take care of
  // deletion without external segfaults
  FConfig(const double &time_s_, const unsigned &ecalc_nb_samples_,
          ECalc *ecalc_, simstrategy_t *simulation_strat_,
          IBackpropagationStrategy *decision_backprop_strat_,
          IBackpropagationStrategy *opponent_backprop_strat_,
          sstrategy_t *decision_selection_strat_,
          sstrategy_t *opponent_selection_strat_,
          sstrategy_t *move_select_strat_, const bool &seed = true);

  ~FConfig();

  // ----------------------------------------------------------------------
  /// @brief   returns the model with the given name
  ///
  /// @param name of model
  ///
  /// @return pointer to model or NULL if model not found.
  // ----------------------------------------------------------------------
  IModel *model(const string &name);

  // ----------------------------------------------------------------------
  /// @brief   parse models from json.
  ///
  /// @param data json 
  // ----------------------------------------------------------------------
  void readModels(const Value &data);

  // ----------------------------------------------------------------------
  /// @brief   gets the number generator
  ///
  /// @return nb gen
  // ----------------------------------------------------------------------
  generator_t *nb_gen();

  sstrategy_t *move_selector(){ return move_selection_strat; }
};
}

#endif
