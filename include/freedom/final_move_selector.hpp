/*
 * File:   final_move_selector.h
 * Author: batman
 *
 * Created on April 11, 2014, 10:34 PM
 */

#ifndef FINAL_MOVE_SELECTOR_H
#define FINAL_MOVE_SELECTOR_H

#include <mcts/iselection_strategy.hpp>
#include <mcts/max_value_selector.hpp>

namespace freedom {
    template<typename Context, typename Config>
class FinalMoveSelector : public ISelectionStrategy<Context, Config> {
private:
  typedef typename INode<Context, Config>::node_t node_t;

  double ev_threshold;
  double big_raise_ev_multiplicator;
  double big_raise_multiplicator;

public:
  /**
   * @param _ev_threshold   minimal threshold for actions that cost money
   * @param _big_raise_ev_multiplicator when ev of action is > then this
   * multiplicator * cost of action then a big raise is employed
   * @param _big_raise_multiplicator when big raise is employed, big raise is
   * this multiplicator * normal raise size
   */
  FinalMoveSelector(double _ev_threshold, double _big_raise_ev_multiplicator,
                    double _big_raise_multiplicator)
      : ev_threshold(_ev_threshold),
        big_raise_ev_multiplicator(_big_raise_ev_multiplicator),
        big_raise_multiplicator(_big_raise_multiplicator) {}

  node_t *select(node_t *node) {
    // find max node
    vector<node_t *> children = node->children();
    FContext context = node->context();
    MaxValueSelector<Context, Config> max_selector;
    node_t *max_node = max_selector.select(node);

    //for(auto c : children)
        //std::cout << c->get_ev() << std::endl;
    //std::cout << std::endl;

    // split in actions with amount > zero and eq zero
    vector<node_t *> wamt, noamt;
    for (int i = 0; i < children.size(); ++i) {
      FContext c = children[i]->context();
      if (c.last_action.amount == bb(0))
        noamt.push_back(children[i]);
      else
        wamt.push_back(children[i]);
    }

    // sort wamt max ev first to loop through them
    // greatest ev first
    struct {
      bool operator()(node_t *x, node_t *y) {
        return (x->ev() > y->ev());
      }
    } comparator;
    std::sort(wamt.begin(), wamt.end(), comparator);

    // check each action for the threshold. if below, try another action
    for (int i = 0; i < wamt.size(); ++i) {
      FContext c = wamt[i]->context();
      double ratio = wamt[i]->ev() / c.last_action.amount.getAsDouble();
      //std::cout << ratio << std::endl;
      // check if maxnode >= threshold
      if (ratio >= ev_threshold) {
        // if raise, check if we should raise big
        FContext mc = wamt[i]->context();
        if (mc.last_action.action == ActionType::Raise &&
            wamt[i]->ev() >= (mc.last_action.amount.getAsDouble() *
                                  big_raise_ev_multiplicator)) {
          mc.last_action.amount *= bb(big_raise_multiplicator);
        }

        return wamt[i];
      }
    }

    // take a "free" action (check/fold)
    return noamt.back();
  }
};
}

#endif /* FINAL_MOVE_SELECTOR_H */
