#ifndef MODEL_SELECTOR_H
#define MODEL_SELECTOR_H

#include <mcts/iselection_strategy.hpp>
#include "imodel.hpp"
#include <iostream>

namespace freedom {

using mcts::INode;
using mcts::ISelectionStrategy;

// ----------------------------------------------------------------------
/// @brief   wraps an action and node with a probability value
// ----------------------------------------------------------------------
template <typename Config> struct PAction {
  typedef typename INode<FContext, Config>::node_t node_t;
  typedef PAction<Config> paction_t;

  node_t *node;
  Action action;
  double prob;

  PAction() : node(NULL), action(Action(ActionType::None, bb(0))), prob(0) {}
  PAction(node_t *node_, const Action &action_, const double &prob_)
      : node(node_), action(action_), prob(prob_) {}
};

// ----------------------------------------------------------------------
/// @brief   The Model selector looks up a model to make
///          an informed decision which node to follow based
///          on the probabilities of each action.
///
/// @tparam Config a configuration object
// ----------------------------------------------------------------------
template <typename Config>
class ModelSelector : public ISelectionStrategy<FContext, Config> {
  typedef typename INode<FContext, Config>::node_t node_t;
  typedef typename PAction<Config>::paction_t paction_t;
  typedef std::mt19937 generator_t;

public:
  node_t *select(node_t *node) {
    vector<double> probs;
    vector<paction_t> actions = normalized_probabilities(node);
    for (auto a : actions)
      probs.push_back(a.prob);
    return node->children().at(
        choose_discrete_index(*node->config()->nb_gen(), probs));
  }

  vector<paction_t> normalized_probabilities(node_t *node) {
    vector<node_t *> children = node->children();
    Config *config = node->config();
    FContext context = node->context();
    string model_name = context.active_player().model;
    IModel *model = config->model(model_name);

    // stores node probabilities
    vector<paction_t> actions;

    double prob;
    for (auto c : children) {
      FContext ccontext = c->context();
      Action action = ccontext.last_action;
      prob = model->get_action_probability(action, context.phase,
                                           context.betting_round);
      actions.push_back(paction_t(c, action, prob));
    }

    return actions;
  }

  /**
   * input is a number of probabilities that should add up
   * to 1.
   * output is an index of that vector that was choosen
   * accordingly to the distribution
   */
  int choose_discrete_index(generator_t &gen, const vector<double> &probs) {
    std::discrete_distribution<int> distribution(probs.begin(), probs.end());
    //vector<double> p = distribution.probabilities();
    //for(int i = 0; i < p.size(); i++)
        //std::cout << p[i] << std::endl;
    //std::cout << "\n";
    //for(int i = 0; i < 10; i++)
    //std::cout <<distribution(gen) << std::endl; 
    return distribution(gen);
  }
};
}

#endif
