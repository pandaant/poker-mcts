/*
 * File:   model_selector.h
 * Author: batman
 *
 * Created on November 19, 2013, 12:20 AM
 */

#ifndef MODEL_SELECTOR_H
#define MODEL_SELECTOR_H

#include <mcts/iselection_strategy.hpp>
#include "model.hpp"

namespace freedom {
using namespace mcts;

// wraps a action and a probability value
template <typename Context, typename Config>
struct PAction {
  typedef typename INode<Context, Config>::node_t node_t;

  node_t *node;
  Action action;
  double prob;

  PAction() : node(NULL), action(Action(ActionType::None, bb(0))), prob(0) {}
  PAction(node_t *_node, Action _action, double _prob)
      : node(_node), action(_action), prob(_prob) {}
};

/**
 * The Model selector looks up a model to make
 * an informed decision which node to follow based
 * on the probabilities of each action.
 */
template <typename Context, typename Config>
class ModelSelector : public ISelectionStrategy<Context, Config> {
  typedef typename INode<Context, Config>::node_t node_t;
  typedef std::mt19937 generator_t;

public:
  ModelSelector() {}

  node_t *select(node_t *node) {
    vector<double> probs;
    vector<PAction<Context,Config>> actions = normalized_probabilities(node);
    for (auto a : actions)
      probs.push_back(a.prob);
    return node->children().at(
        choose_discrete_index(*node->config()->nb_gen(), probs));
  }

  vector<PAction<Context,Config>> normalized_probabilities(node_t *node) {
    vector<node_t *> children = node->children();
    Config* config = node->config();
    Context context = node->context();
    string model_name = context.active_seat().model;
    Model *model = config->model(model_name);

    // stores node probabilities
    vector<PAction<Context,Config>> actions;

    double prob;
    for (auto c : children) {
      Context ccontext = c->context();
      Action action = ccontext.last_action;
      prob = model->get_action_probability(action, context.phase, context.betting_round);
      actions.push_back(PAction<Context,Config>(c, action, prob));
    }

    return actions;
  }

  /**
   * input is a number of probabilities that should add up
   * to 1.
   * output is an index of that vector that was choosen
   * accordingly to the distribution
   */
  int choose_discrete_index(generator_t &gen, vector<double> probs) {
    std::discrete_distribution<int> distribution(probs.begin(), probs.end());
    return distribution(gen);
  }
};
}

#endif /* MODEL_SELECTOR_H */

