#include "histogramm.hpp"
#include <iostream>

namespace Freedom5 {

    Histogramm::Histogramm() {}

    Histogramm::Histogramm(string _name, vector<PhaseHistogramm> _histogramm)
        : Model(_name), histogramm(_histogramm) {}

    Histogramm::Histogramm(const Histogramm &oh) {
        name = oh.name;
        histogramm = oh.histogramm;
    }

    Histogramm::Histogramm(const Value &data) {
        name = data["name"].GetString();
        for (SizeType i = 0; i < data["histogramm"].Size(); ++i)
            histogramm.push_back(PhaseHistogramm(data["histogramm"][i]));
    }

    tuple<double, double, double>
        Histogramm::getFoldCallRaiseProbabilities(const FContext &gs, int index_seat) const {
            PhaseType::Enum phase = gs.phase;
            int btr = gs.betting_round;
            return histogramm[phase].round_data[btr].getFoldCallRaiseProbabilities();
        }

    tuple<double, double>
        Histogramm::getCheckBetProbabilities(const FContext &gs, int index_seat) const {
            PhaseType::Enum phase = gs.phase;
            int btr = gs.betting_round;
            return histogramm[phase].round_data[btr].getCheckBetProbabilities();
        }

    tuple<double, double>
        Histogramm::getFoldCallProbabilities(const FContext &gs, int index_seat) const {
            PhaseType::Enum phase = gs.phase;
            int btr = gs.betting_round;
            return histogramm[phase].round_data[btr].getFoldCallProbabilities();
        }

    double Histogramm::get_action_probability(Action action,
            PhaseType::Enum phase, int betting_round) const {
        // TODO
        // very temporary fix, this has to be handled in phase_histogramm constructor
        if (betting_round > histogramm[phase].round_data.size() - 1)
            return 0;

        //TODO ACTIONS TO MODEL ACTIONS ( RAISE is not split in bet/raise )
        /**
         * some parts of the bot care about bet/raise difference and some dont.
         * it should change raise to bet if btround = 0
         **/
        if( betting_round == 0 &&
                action.action == ActionType::Raise)
            action.action = ActionType::Bet;

        // aquire the right round histogramm
        RoundHistogramm round_hist =
            histogramm[phase].round_data[betting_round];
        return round_hist.get_action_probability(action.action);
    }
};
