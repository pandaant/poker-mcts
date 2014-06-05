#ifndef SHOWDOWN_EVAL_STRATEGY_H
#define SHOWDOWN_EVAL_STRATEGY_H

#include <poker/pokerdefs.hpp>
#include <mcts/isimulation_strategy.hpp>
#include <ecalc/ecalc.hpp>
#include "fcontext.hpp"

namespace freedom {
using ecalc::ECalc;

class ShowdownEval : public mcts::ISimulationStrategy<FContext> {
protected:
  ECalc *ecalc;
  unsigned samples;

public:
  ShowdownEval(ECalc *_ecalc, const unsigned &samples_);

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
  void set_ecalc(ECalc *ecalc_, const unsigned &samples_);
};
}

#endif
