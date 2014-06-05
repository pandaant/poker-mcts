#include "opponent_node.hpp"
#include "const_leaf_node.hpp"
#include "showdown_eval.hpp"
#include "showdown_node.hpp"

namespace freedom {

OpponentNode::OpponentNode(const FContext &context_, FConfig *config_,
                           node_t *parent_)
    : InnerNode<FContext, FConfig>(context_, config_, parent_,
                                   config_->opponent_backprop_strat->create()) {
}

void OpponentNode::expand() {
  vector<FContext> transitions = this->context().transition();

  std::for_each(transitions.begin(), transitions.end(), [&](FContext &c) {
    ShowdownEval *sim_strat =
        static_cast<ShowdownEval *>(this->config()->simulation_strat);

    if (c.is_terminal()) {
      // bot last remaining player
      if (!c.bot_seat().is_inactive() && c.nb_player_not_inactive() == 1) {
        bb ev = sim_strat->winnable_pot(c);
        bb our_investment =
            c.bot_seat().total_investment() - sim_strat->get_fixed_win(c);
        ev -= our_investment;
        // subtract rake
        ev *= bb(1 - c.config->rake_factor);
        this->add_child(
            new ConstantLeafNode(c, this->config(), this, ev.getAsDouble()));
      } else if (c.nb_player_not_inactive() >= 2) {
        // more than two players at showdown
        this->add_child(new ShowdownNode(c, this->config(), this));
      }
    } else {
      if (c.index_bot == c.index_active)
        this->add_child(new DecisionNode(c, this->config(), this));
      else
        this->add_child(new OpponentNode(c, this->config(), this));
    }
  });
}

INode<FContext, FConfig>::node_t *OpponentNode::select_child() {
  return this->config()->opponent_selection_strat->select(this);
}

OpponentNode::~OpponentNode() {
  //        delete static_cast<FContext*>(context);
}
};
