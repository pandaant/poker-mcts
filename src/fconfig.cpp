#include <map>
#include "fconfig.hpp"
#include "showdown_eval.hpp"
#include "weighted_showdown_eval.hpp"
#include "histogramm.hpp"

namespace freedom {

FConfig::FConfig(const double &time_s_, const unsigned &ecalc_nb_samples_,
                 ECalc *ecalc_, simstrategy_t *simulation_strat_,
                 IBackpropagationStrategy *decision_backprop_strat_,
                 IBackpropagationStrategy *opponent_backprop_strat_,
                 sstrategy_t *decision_selection_strat_,
                 sstrategy_t *opponent_selection_strat_,
                 sstrategy_t *move_select_strat_, const bool &seed)
    : // TODO seed gen
      nb_gen_(new generator_t(0)),
      time_s(time_s_), ecalc_nb_samples(ecalc_nb_samples_), ecalc(ecalc_),
      simulation_strat(simulation_strat_),
      opponent_selection_strat(opponent_selection_strat_),
      decision_backprop_strat(decision_backprop_strat_),
      opponent_backprop_strat(opponent_backprop_strat_),
      move_selection_strat(move_select_strat_),
      decision_selection_strat(decision_selection_strat_) {}

FConfig::FConfig(const Value &data, ECalc *ecalc_, const bool &seed)
    : ecalc(ecalc_), time_s(data["time"].GetDouble()),
      ecalc_nb_samples(data["ecalc_nb_samples"].GetInt()),
      simulation_strat(new WeightedShowdownEval(ecalc, ecalc_nb_samples)),
      decision_backprop_strat(StrategyMap::lookup_backprop_strat(
          data["decision_backprop"].GetString())),
      opponent_backprop_strat(StrategyMap::lookup_backprop_strat(
          data["opponent_backprop"].GetString())),
      decision_selection_strat(
          StrategyMap::lookup_selection_strat<FContext, FConfig>(
              data["decision_selection"].GetString())),
      opponent_selection_strat(
          StrategyMap::lookup_selection_strat<FContext, FConfig>(
              data["opponent_selection"].GetString())),
      move_selection_strat(
          StrategyMap::lookup_selection_strat<FContext, FConfig>(
              data["move_selection"].GetString())) {}

FConfig::~FConfig() {
  //      delete move_selection_strat;
  model_map::iterator iter;
  for (iter = models.begin(); iter != models.end(); ++iter) {
    delete iter->second;
  }
  /*delete simulation_strat;
  delete decision_backprop_strat;
  delete opponent_backprop_strat;
  delete decision_selection_strat;
  delete opponent_selection_strat;*/
}

void FConfig::readModels(const Value &data) {
  for (SizeType i = 0; i < data.Size(); ++i)
    models[data[i]["name"].GetString()] = new Histogramm(data[i]);
}

IModel *FConfig::model(const string &name) { return models[name]; }

FConfig::generator_t *FConfig::nb_gen() { return nb_gen_; }
};
