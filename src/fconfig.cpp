#include <map> 
#include "fconfig.hpp"
#include "showdown_eval.hpp"
#include "weighted_showdown_eval.hpp"
#include "histogramm.hpp"

namespace Freedom5 {

    FConfig::FConfig(const Value &data, ECalc *_ecalc, bool seed){
        ecalc = _ecalc;
        time_s = data["time"].GetDouble();
        dump_tree = data["dump_tree"].GetBool();
        ecalc_nb_samples = data["ecalc_nb_samples"].GetInt();

        simulation_strat = new WeightedShowdownEval(ecalc,ecalc_nb_samples);
        //simulation_strat = new ShowdownEval(ecalc,ecalc_nb_samples);
        decision_backprop_strat =
                StrategyMap::lookup_backprop_strat(data["decision_backprop"].GetString());
        opponent_backprop_strat =
                StrategyMap::lookup_backprop_strat(data["opponent_backprop"].GetString());
        decision_selection_strat = StrategyMap::lookup_selection_strat<FContext, FConfig>(
                data["decision_selection"].GetString());
        opponent_selection_strat = StrategyMap::lookup_selection_strat<FContext, FConfig>(
                data["opponent_selection"].GetString());
        move_selection_strat_ = StrategyMap::lookup_selection_strat<FContext, FConfig>(data["move_selection"].GetString());
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
    
    Model* FConfig::model(string name){
        return models[name];
    }
};
