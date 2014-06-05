#ifndef OPPONENT_NODE_H
#define OPPONENT_NODE_H

#include <mcts/inner_node.hpp>
#include "decision_node.hpp"

namespace freedom {

using mcts::InnerNode;

/**
 * OpponentNode
 * Uses a selectionStrategy for nodes
 * in which an opponent has to make a move.
 **/
class OpponentNode : public InnerNode<FContext, FConfig> {
  typedef typename INode<FContext, FConfig>::node_t node_t;

public:
  OpponentNode(const FContext &context_, FConfig *config_, node_t *parent_);
  virtual ~OpponentNode();

  void expand();
  virtual node_t *select_child();
};
}

#endif
