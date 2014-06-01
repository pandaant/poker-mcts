/*
 * File:   const_leaf_node.h
 * Author: batman
 *
 * Created on August 6, 2013, 12:34 PM
 */

#ifndef CONST_LEAF_NODE_H
#define CONST_LEAF_NODE_H

#include <mcts/inode.hpp>
#include <mcts/leaf_node.hpp>
#include "fconfig.hpp"
#include "fcontext.hpp"

namespace Freedom5 {

using mcts::LeafNode;

/**
 * ConstantLeafNode
 * An leaf node with an exact value
 * if the game ends without an showdown.
 **/
class ConstantLeafNode : public LeafNode<FContext, FConfig> {
  typedef typename INode<FContext, FConfig>::node_t node_t;

public:
  double value;

  ConstantLeafNode(const FContext &_FContext, FConfig *_FConfig, node_t *_parent,
                   double _value)
      : LeafNode<FContext, FConfig>(_FContext, _FConfig, _parent), value(_value) {}
  ~ConstantLeafNode() {}

  virtual double ev() const { return value; }

  virtual double std_dev() const { return 0; }

  virtual double variance() const { return 0; }

  virtual double simulate() { return value; }

  virtual void backpropagate(const double &value) {
    ++this->nb_samples_;
    this->parent_->backpropagate(value);
  }
};
};

#endif /* CONST_LEAF_NODE_H */

