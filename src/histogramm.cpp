#include "histogramm.hpp"

namespace freedom {

Histogramm::Histogramm(vector<PhaseHistogramm> _histogramm)
    : histogramm(_histogramm) {}

Histogramm::Histogramm(const Histogramm &oh) : histogramm(oh.histogramm) {}

Histogramm::Histogramm(const Value &data) {
  for (SizeType i = 0; i < data["histogramm"].Size(); ++i)
    histogramm.push_back(PhaseHistogramm(data["histogramm"][i]));
}

Histogramm::~Histogramm() {}

tuple<double, double, double>
Histogramm::getFoldCallRaiseProbabilities(const FContext &gs) const {
  return histogramm[gs.phase]
      .round_data[gs.betting_round]
      .getFoldCallRaiseProbabilities();
}

tuple<double, double>
Histogramm::getCheckBetProbabilities(const FContext &gs) const {
  return histogramm[gs.phase]
      .round_data[gs.betting_round]
      .getCheckBetProbabilities();
}

tuple<double, double>
Histogramm::getFoldCallProbabilities(const FContext &gs) const {
  return histogramm[gs.phase]
      .round_data[gs.betting_round]
      .getFoldCallProbabilities();
}

double Histogramm::get_action_probability(const Action &action,
                                          const PhaseType::Enum &phase,
                                          const unsigned &betting_round) const {
  // TODO
  // very temporary fix, this has to be handled in phase_histogramm constructor
  if (betting_round > histogramm[phase].round_data.size() - 1)
    return 0;

  // some parts of the bot care about bet/raise difference and some dont.
  // it should change raise to bet if btround = 0
  ActionType::Enum ac =
      (betting_round == 0 && action.action == ActionType::Raise)
          ? ActionType::Bet
          : action.action;

  // aquire the right round histogramm
  RoundHistogramm round_hist = histogramm[phase].round_data[betting_round];
  return round_hist.get_action_probability(ac);
}

IModel *Histogramm::clone() const { return new Histogramm(*this); }
};
