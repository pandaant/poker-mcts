/*
 * File:   showdown_eval.h
 * Author: batman
 *
 * Created on August 10, 2013, 10:19 AM
 */

#ifndef SHOWDOWN_EVAL_STRATEGY_H
#define SHOWDOWN_EVAL_STRATEGY_H

#include <poker/pokerdefs.hpp>
#include <mcts/isimulation_strategy.hpp>
#include <ecalc/ecalc.hpp>
#include "fcontext.hpp"

namespace Freedom5 {
using ecalc::ECalc;

class ShowdownEval : public mcts::ISimulationStrategy<FContext> {
protected:
  ECalc *ecalc;
  int samples;

public:
  ShowdownEval() {}
  ShowdownEval(ECalc *_ecalc, int _samples);
  virtual ~ShowdownEval();
  virtual double simulate(const FContext &FContext);

  /**
   * if we bet more than anybody else can pay, we get it back
   * because it is not winnable by anybody else
   * @param gs
   * @return
   */
  bb get_fixed_win(const FContext &FContext) const;

  /**
   * if bot could not commit enough money to make the same investment
   * as someone else. this difference can't be won by the bot.
   * @param FContext
   * @return
   */
  bb get_not_winnable_pot(const FContext &FContext) const;
  bb winnable_pot(const FContext &FContext) const;
  void set_ecalc(ECalc *_ecalc, int _samples);
};
};

#endif /* SIMPLE_SIM_STRATEGY_H */

