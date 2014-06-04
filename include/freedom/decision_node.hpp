/*
 * File:   decision_node.h
 * Author: batman
 *
 * Created on August 6, 2013, 12:34 PM
 */

#ifndef DECISION_NODE_H
#define DECISION_NODE_H

#include <mcts/inner_node.hpp>
#include "fconfig.hpp"
#include "fcontext.hpp"

namespace freedom {

using mcts::INode;
using mcts::InnerNode;

/**
 * DecisionNode
 * Uses a selectionStrategy for nodes
 * in which we (the bot) have to make a move.
 **/
class DecisionNode : public InnerNode<FContext, FConfig> {
  typedef typename INode<FContext, FConfig>::node_t node_t;
public:
  DecisionNode(const FContext &_FContext, FConfig *_FConfig, node_t *_parent)
      : InnerNode<FContext, FConfig>(_FContext, _FConfig, _parent,
                  _FConfig->decision_backprop_strat->create()) {}

  void expand();
  virtual node_t *select_child();
  virtual ~DecisionNode();
};
};

#endif /* DECISION_NODE_H */

