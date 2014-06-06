#ifndef DECISION_NODE_H
#define DECISION_NODE_H

#include "fconfig.hpp"
#include "fcontext.hpp"
#include <mcts/inner_node.hpp>

namespace freedom {

using mcts::INode;
using mcts::InnerNode;

// ----------------------------------------------------------------------
/// @brief   Uses a selectionStrategy for nodes in which we (the bot) 
///          has to make a move.
// ----------------------------------------------------------------------
class DecisionNode : public InnerNode<FContext, FConfig> {
  typedef typename INode<FContext, FConfig>::node_t node_t;

public:
  DecisionNode(const FContext &fcontext_, FConfig *fconfig_, node_t *parent_);

  void expand();
  virtual node_t *select_child();
  virtual ~DecisionNode();
};
}

#endif
