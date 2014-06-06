#ifndef BETAMT_EV_RATIO_SELECTOR_H
#define BETAMT_EV_RATIO_SELECTOR_H

#include <mcts/inode.hpp>
#include <mcts/iselection_strategy.hpp>
#include "fcontext.hpp"

namespace freedom {

using mcts::INode;
using mcts::ISelectionStrategy;

class FConfig;

// ----------------------------------------------------------------------
/// @brief   selects node that maximises the ratio between ev
///          and the amount to call/bet with.
// ----------------------------------------------------------------------
class BetamtEVRatioSelector : public ISelectionStrategy<FContext, FConfig> {
  typedef typename INode<FContext, FConfig>::node_t node_t;

public:
  // ----------------------------------------------------------------------
  /// @brief   if the ratio is bigger than threshold, the action is a
  ///          valid candidate. if threshold is not reached. an action
  ///          like fold/check is executed.
  ///
  /// @param threshold_ threshold for valid action
  // ----------------------------------------------------------------------
  explicit BetamtEVRatioSelector(const double &threshold_);

  // ----------------------------------------------------------------------
  /// @brief   selects a childnode of node that fits the ratio best.
  ///
  /// @param node node to select from
  ///
  /// @return childnode
  // ----------------------------------------------------------------------
  node_t *select(node_t *node);

private:
  double threshold;
};
}

#endif
