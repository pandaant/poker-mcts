#ifndef FINAL_MOVE_SELECTOR_H
#define FINAL_MOVE_SELECTOR_H

#include <mcts/iselection_strategy.hpp>
#include <mcts/max_value_selector.hpp>
#include "fcontext.hpp"

namespace freedom {

using mcts::INode;
using mcts::MaxValueSelector;
using mcts::ISelectionStrategy;

class FConfig;

// ----------------------------------------------------------------------
/// @brief   Selects the node for the next action when a result for
///          an MCTS run is needed.
// ----------------------------------------------------------------------
class FinalMoveSelector : public ISelectionStrategy<FContext, FConfig> {
  typedef typename INode<FContext, FConfig>::node_t node_t;

public:
  // ----------------------------------------------------------------------
  /// @brief  constructs a new selector
  ///
  /// @param _ev_threshold minimal threshold for actions that cost money
  /// @param _big_raise_ev_multiplicator when ev of action is > then this
  ///        multiplicator * cost of action then a big raise is employed.
  /// @param _big_raise_multiplicator when big raise is employed, big raise is
  ///        this multiplicator * normal raise size
  // ----------------------------------------------------------------------
  FinalMoveSelector(const double &ev_threshold_,
                    const double &big_raise_ev_multiplicator_,
                    const double &big_raise_multiplicator_);

  node_t *select(node_t *node);

private:
  double ev_threshold;
  double big_raise_ev_multiplicator;
  double big_raise_multiplicator;
};
}

#endif
