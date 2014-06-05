#ifndef EV_AMT_RATIO_SELECTOR_H
#define EV_AMT_RATIO_SELECTOR_H

#include <mcts/inode.hpp>
#include <mcts/max_function_selector.hpp>
#include "fcontext.hpp"

namespace freedom {
using namespace mcts;

template <typename Context, typename Config>
class EVPotRatioSelector : public MaxFunctionSelector<Context, Config> {
  typedef typename INode<Context, Config>::node_t node_t;

public:
  virtual double evaluate(node_t *node) const {
    FContext context = node->context();
    double ev = node->ev();
    bb action_amount = context.last_action.amount;
    bb pot = context.pot + action_amount;
    return ev / pot.getAsDouble();
  }
};
}

#endif
