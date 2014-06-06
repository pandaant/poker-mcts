#include "final_move_selector.hpp"

namespace freedom {
FinalMoveSelector::FinalMoveSelector(const double &ev_threshold_,
                                     const double &big_raise_ev_multiplicator_,
                                     const double &big_raise_multiplicator_)
    : ev_threshold(ev_threshold_),
      big_raise_ev_multiplicator(big_raise_ev_multiplicator_),
      big_raise_multiplicator(big_raise_multiplicator_) {}

FinalMoveSelector::node_t *FinalMoveSelector::select(node_t *node) {
  // find max node
  vector<node_t *> children = node->children();
  FContext context = node->context();
  MaxValueSelector<FContext, FConfig> max_selector;
  // node_t *max_node = max_selector.select(node);

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
    bool operator()(node_t *x, node_t *y) { return (x->ev() > y->ev()); }
  } comparator;
  std::sort(wamt.begin(), wamt.end(), comparator);

  // check each action for the threshold. if below, try another action
  for (int i = 0; i < wamt.size(); ++i) {
    FContext c = wamt[i]->context();
    double ratio = wamt[i]->ev() / c.last_action.amount.getAsDouble();
    // std::cout << ratio << std::endl;
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
}
