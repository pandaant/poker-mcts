#ifndef CONST_LEAF_NODE_H
#define CONST_LEAF_NODE_H

#include "fconfig.hpp"
#include "fcontext.hpp"
#include <mcts/inode.hpp>
#include <mcts/leaf_node.hpp>

namespace freedom {

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

  ConstantLeafNode(const FContext &fcontext_, FConfig *fconfig_,
                   node_t *parent_, const double &value_)
      : LeafNode<FContext, FConfig>(fcontext_, fconfig_, parent_),
        value(value_) {}

  virtual ~ConstantLeafNode() {}

  virtual double ev() const { return value; }

  virtual double std_dev() const { return 0; }

  virtual double variance() const { return 0; }

  virtual double simulate() { return value; }

  virtual void backpropagate(const double &value) {
    ++this->nb_samples_;
    this->parent_->backpropagate(value);
  }
};
}

#endif
