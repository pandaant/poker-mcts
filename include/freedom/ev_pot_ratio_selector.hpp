#ifndef EV_AMT_RATIO_SELECTOR_H
#define EV_AMT_RATIO_SELECTOR_H

#include <mcts/inode.hpp>
#include <mcts/max_function_selector.hpp>
#include "fcontext.hpp"

namespace freedom {
using namespace mcts;

class FConfig;

// ----------------------------------------------------------------------
/// @brief   selects childnode based on the ratio between ev and pot.
// ----------------------------------------------------------------------
class EVPotRatioSelector : public MaxFunctionSelector<FContext, FConfig> {
  typedef typename INode<FContext, FConfig>::node_t node_t;

public:
  virtual double evaluate(node_t *node) const {
    FContext context = node->context();
    bb action_amount = context.last_action.amount;
    bb pot = context.pot + action_amount;
    return node->ev() / pot.getAsDouble();
  }
};
}

#endif
