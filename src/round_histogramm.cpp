#include "round_histogramm.hpp"

namespace freedom {

RoundHistogramm::RoundHistogramm(const double &_nb_fold,
                                 const double &_nb_check,
                                 const double &_nb_call, const double &_nb_bet,
                                 const double &_nb_raise)
    : nb_fold(_nb_fold), nb_check(_nb_check), nb_call(_nb_call),
      nb_bet(_nb_bet), nb_raise(_nb_raise), nb_all_nbet(_nb_check + _nb_bet),
      nb_all_wbet(_nb_call + _nb_fold + _nb_raise) {}

RoundHistogramm::RoundHistogramm(const Value &data)
    : nb_fold(data[0u].GetDouble()), nb_check(data[1u].GetDouble()),
      nb_call(data[2u].GetDouble()), nb_bet(data[3u].GetDouble()),
      nb_raise(data[4u].GetDouble()) {
  nb_all_nbet = nb_check + nb_bet;
  nb_all_wbet = nb_call + nb_fold + nb_raise;
}

tuple<double, double, double>
RoundHistogramm::getFoldCallRaiseProbabilities() const {
  if (nb_all_wbet == 0)
    return make_tuple(0, 0, 0);
  double fold = nb_fold / nb_all_wbet;
  double call = nb_call / nb_all_wbet;
  double raise = nb_raise / nb_all_wbet;
  return make_tuple(fold, call, raise);
}

tuple<double, double> RoundHistogramm::getCheckBetProbabilities() const {
  if (nb_all_nbet == 0)
    return make_tuple(0, 0);
  double check = nb_check / nb_all_nbet;
  double bet = nb_bet / nb_all_nbet;
  return make_tuple(check, bet);
}

tuple<double, double> RoundHistogramm::getFoldCallProbabilities() const {
  if (nb_all_wbet == 0)
    return make_tuple(0, 0);
  double fold = nb_fold / nb_all_wbet;
  double call = nb_call / nb_all_wbet;
  return make_tuple(fold, call);
}

double
RoundHistogramm::get_action_probability(const ActionType::Enum &action) const {
  using namespace ActionType;
  double all = (action == Bet || action == Check) ? nb_all_nbet : nb_all_wbet;

  // no data for this action, so prob is zero.
  // floatingpoint zero comparision
  if (all < 1e-10)
    return 0;
  return get(action) / all;
}

double RoundHistogramm::get(const ActionType::Enum &action) const {
  using namespace ActionType;
  switch (action) {
  case Fold:
    return nb_fold;
  case Check:
    return nb_check;
  case Call:
    return nb_call;
  case Bet:
    return nb_bet;
  case Raise:
  case AllIn:
    return nb_raise;
  default:
    return 0;
  }
}
}
