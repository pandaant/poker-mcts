#include <algorithm>
#include <iostream>
#include "showdown_eval.hpp"
#include <ecalc/ecalc.hpp>
#include <ecalc/single_handlist.hpp>
#include <ecalc/array_handlist.hpp>
#include <ecalc/random_handlist.hpp>

namespace freedom {

using namespace poker;

ShowdownEval::ShowdownEval(ECalc *ecalc_, const unsigned &samples_)
    : ecalc(ecalc_), samples(samples_) {}

ShowdownEval::~ShowdownEval() {}

double ShowdownEval::simulate(const FContext &context) {
  vector<Handlist *> lists(0);
  int index_bot = context.index_bot;
  int bot_result_index = -1;

  // get active players handlists
  for (unsigned i = 0; i < context.player.size(); ++i) {
    if (!context.player[i].is_inactive()) {
      lists.push_back(context.player[i].handlist);
      // save relative bot index for retrival
      if (index_bot == (int)i)
        bot_result_index = lists.size() - 1;
    }
  }

  ecalc::result_collection res;
  res = ecalc->evaluate(lists, context.config->board, vector<unsigned>(),
                        samples);
  ecalc::Result bot_result = res[bot_result_index];

  bb fixedreturn = get_fixed_win(context);
  bb notwinnable = get_not_winnable_pot(context);
  bb winnable_pot = context.pot - fixedreturn - notwinnable;

  double ev = bot_result.pwin_tie() * winnable_pot.getAsDouble();
  // double tie_ev = ptie * (winnable_pot.getAsDouble() / res.size());
  // ev += tie_ev;

  // subtract our investment to the pot
  bb our_investment = context.bot_player().total_investment() - fixedreturn;
  ev -= our_investment.getAsDouble();

  return (1 - context.config->rake_factor) * ev;
}

bb ShowdownEval::get_fixed_win(const FContext &context) const {
  bb bot_investment;
  vector<bb> investments;

  for (unsigned i = 0; i < context.player.size(); ++i) {
    if ((int)i == context.index_bot)
      bot_investment = context.player[i].total_investment();
    else
      investments.push_back(context.player[i].total_investment());
  }

  // get biggest opponent investment
  bb max = *std::max_element(investments.begin(), investments.end());

  /**
   * if max is <= to our investment, we play for everything, no payback
   * else we get the difference back
   **/
  return (bot_investment <= max) ? bb(0) : (bot_investment - max);
}

bb ShowdownEval::get_not_winnable_pot(const FContext &context) const {
  bb bot_investment;
  vector<bb> investments;

  for (unsigned i = 0; i < context.player.size(); ++i) {
    if ((int)i == context.index_bot)
      bot_investment = context.player[i].total_investment();
    else
      investments.push_back(context.player[i].total_investment());
  }

  bb deficit(0);
  bb not_winnable(0);
  for (unsigned i = 0; i < investments.size(); ++i) {
    deficit = investments[i] - bot_investment;
    not_winnable += (deficit > bb(0)) ? deficit : bb(0);
  }

  // std::cout << bot_investment << "\n";
  return not_winnable;
}

bb ShowdownEval::winnable_pot(const FContext &context) const {
  bb pot = context.pot;
  return pot - get_fixed_win(context) - get_not_winnable_pot(context);
}

void ShowdownEval::set_ecalc(ECalc *ecalc_, const unsigned &samples_) {
  ecalc = ecalc_;
  samples = samples_;
}
};
