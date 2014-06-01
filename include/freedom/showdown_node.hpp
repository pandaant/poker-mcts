/*
 * File:   showdown_node.h
 * Author: batman
 *
 * Created on August 6, 2013, 12:35 PM
 */

#ifndef SHOWDOWN_NODE_H
#define SHOWDOWN_NODE_H

#include <mcts/leaf_node.hpp>
#include <mcts/isimulation_strategy.hpp>
#include <mcts/running_stats.hpp>

namespace Freedom5 {

using mcts::LeafNode;
using mcts::RunningStats;

/**
 * ShowdownNode
 * Represents the end of the game.
 * An simulation Strategy is applied to
 * determine the payoffs for the players
 **/
class ShowdownNode : public LeafNode<FContext, FConfig> {
  typedef typename INode<FContext, FConfig>::node_t node_t;
public:
  RunningStats stats;

  ShowdownNode(const FContext &_context, FConfig *_config, node_t *_parent)
      : LeafNode<FContext, FConfig>(_context, _config, _parent), stats() {}

  virtual double simulate() {
    return this->config_->simulation_strat->simulate(this->context_);
  }

  virtual double ev() const { return stats.mean(); }

  double std_dev() const { return stats.standard_deviation(); }

  double variance() const { return stats.variance(); }

  int nb_samples() const { return stats.num_data_values(); }

  virtual void backpropagate(const double &value) {
    stats.push(value);
    this->parent_->backpropagate(value);
  }

  virtual ~ShowdownNode(){}
};
};

#endif /* SHOWDOWN_NODE_H */

