/*
 * File:   round_histogramm.h
 * Author: batman
 *
 * Created on September 4, 2013, 11:00 AM
 */

#ifndef ROUND_HISTOGRAMM_H
#define ROUND_HISTOGRAMM_H

#include <tuple>
#include <rapidjson/document.h>
#include "fcontext.hpp"

namespace Freedom5 {

using rapidjson::Value;
using std::get;
using std::tuple;
using std::make_tuple;

class RoundHistogramm {
public:
  double nb_fold;
  double nb_check;
  double nb_call;
  double nb_bet;
  double nb_raise;
  double nb_all_wbet; // with bet
  double nb_all_nbet; // no bet

  RoundHistogramm(double _nb_fold = 0, double _nb_check = 0, double _nb_call = 0,
                  double _nb_bet = 0, double _nb_raise = 0);
  RoundHistogramm(const Value &data);

  tuple<double, double, double> getFoldCallRaiseProbabilities() const;
  tuple<double, double> getCheckBetProbabilities() const;
  tuple<double, double> getFoldCallProbabilities() const;

  /**
   * ATTENTION
   * this function works only errorfree for postflop evaluations
   * because the special case on prflop is not handled. 
   * When nobody bets the bigblind can X/R but the function
   * will return values for X/B
   **/
  double get_action_probability(ActionType::Enum action);

  // returns number of times this action was taken
  double get(ActionType::Enum action);
};
};

#endif /* ROUND_HISTOGRAMM_H */

