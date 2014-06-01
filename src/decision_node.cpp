#include "decision_node.hpp"
#include "const_leaf_node.hpp"
#include "opponent_node.hpp"
#include "showdown_node.hpp"

namespace Freedom5 {

void DecisionNode::expand() {
  vector<FContext> transitions = this->context().transition();

  std::for_each(transitions.begin(), transitions.end(), [&](FContext &c) {
    if (c.is_terminal()) {
      // bot folded
      if (c.last_action.action == ActionType::Fold) {
        // we get nothing
        this->add_child(new ConstantLeafNode(c, this->config(), this, 0));
      } else if (c.nb_player_active() + c.nb_player_allin() >= 2) {
        // more than two players at showdown
        this->add_child(new ShowdownNode(c, this->config(), this));
      }
    } else {
      // non terminal node
      if (c.index_bot == c.index_active)
        this->add_child(new DecisionNode(c, this->config(), this));
      else
        this->add_child(new OpponentNode(c, this->config(), this));
    }
  });
}

INode<FContext,FConfig>::node_t *DecisionNode::select_child() {
  return this->config()->decision_selection_strat->select((INode *)this);
}

DecisionNode::~DecisionNode() { }
};
