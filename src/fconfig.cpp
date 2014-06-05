#include <map>
#include "fconfig.hpp"
#include "showdown_eval.hpp"
#include "weighted_showdown_eval.hpp"
#include "histogramm.hpp"

namespace freedom {
FConfig::FConfig(sstrategy_t *_move_select_strat, bool seed, bool _dump_tree)
    : move_selection_strat_(_move_select_strat), dump_tree(_dump_tree),
      nb_gen_(new generator_t(0)) {} // TODO seed generator

FConfig::FConfig(double _time_s, int _ecalc_nb_samples, ECalc *_ecalc,
                 simstrategy_t *_simulation_strat,
                 IBackpropagationStrategy *_decision_backprop_strat,
                 IBackpropagationStrategy *_opponent_backprop_strat,
                 sstrategy_t *_decision_selection_strat,
                 sstrategy_t *_opponent_selection_strat,
                 sstrategy_t *_move_select_strat, bool seed, bool _dump_tree)
    : move_selection_strat_(_move_select_strat),
      // TODO seed gen
      nb_gen_(new generator_t(0)), time_s(_time_s),
      ecalc_nb_samples(_ecalc_nb_samples), ecalc(_ecalc),
      simulation_strat(_simulation_strat),
      decision_backprop_strat(_decision_backprop_strat),
      opponent_backprop_strat(_opponent_backprop_strat),
      decision_selection_strat(_decision_selection_strat),
      opponent_selection_strat(_opponent_selection_strat),
      dump_tree(_dump_tree) {}

FConfig::FConfig(const Value &data, ECalc *_ecalc, bool seed) {
  ecalc = _ecalc;
  time_s = data["time"].GetDouble();
  dump_tree = data["dump_tree"].GetBool();
  ecalc_nb_samples = data["ecalc_nb_samples"].GetInt();

  simulation_strat = new WeightedShowdownEval(ecalc, ecalc_nb_samples);
  // simulation_strat = new ShowdownEval(ecalc,ecalc_nb_samples);
  decision_backprop_strat =
      StrategyMap::lookup_backprop_strat(data["decision_backprop"].GetString());
  opponent_backprop_strat =
      StrategyMap::lookup_backprop_strat(data["opponent_backprop"].GetString());
  decision_selection_strat =
      StrategyMap::lookup_selection_strat<FContext, FConfig>(
          data["decision_selection"].GetString());
  opponent_selection_strat =
      StrategyMap::lookup_selection_strat<FContext, FConfig>(
          data["opponent_selection"].GetString());
  move_selection_strat_ =
      StrategyMap::lookup_selection_strat<FContext, FConfig>(
          data["move_selection"].GetString());
}

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

IModel *FConfig::model(string name) { return models[name]; }

FConfig::generator_t *FConfig::nb_gen() { return nb_gen_; }
FConfig::sstrategy_t *FConfig::move_selection_strat() { return move_selection_strat_; }
};
