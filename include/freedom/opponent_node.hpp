/*
 * File:   opponent_node.h
 * Author: batman
 *
 * Created on August 6, 2013, 12:34 PM
 */

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
  OpponentNode(const FContext &_context, FConfig *_config, node_t *_parent)
      : InnerNode<FContext, FConfig>(
            _context, _config, _parent,
            _config->opponent_backprop_strat->create()) {}
  virtual ~OpponentNode();

  void expand();
  virtual node_t *select_child();
};
};

#endif /* OPPONENT_NODE_H */

