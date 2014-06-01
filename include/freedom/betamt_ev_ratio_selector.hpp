/* 
 * File:   betamt_ev_ratio_selector.h
 * Author: batman
 *
 * Created on March 31, 2014, 11:32 AM
 */

#ifndef BETAMT_EV_RATIO_SELECTOR_H
#define	BETAMT_EV_RATIO_SELECTOR_H

#include <mcts/inode.hpp>
#include <mcts/iselection_strategy.hpp>
#include "fcontext.hpp"

namespace Freedom5 {
using namespace mcts;

template<typename Context, typename Config>
class BetamtEVRatioSelector : public ISelectionStrategy<Context, Config> {
    typedef typename INode<Context, Config>::node_t node_t;
    double threshold;

public:
  BetamtEVRatioSelector(double _threshold) : threshold(_threshold) {}

  node_t *select(node_t *node) {
    vector<node_t *> children = node->children();
    FContext context = node->context();

    //for (int i = 0; i < children.size(); ++i)
      //std::cout << static_cast<FContext*>(children[i]->context())->last_action.to_str_w_amt()
                //<< " " << children[i]->ev()  << std::endl;

    // split in actions with amount > zero and eq zero
    vector<node_t *> wamt, noamt;
    for (int i = 0; i < children.size(); ++i) {
      FContext c = children[i]->context();
      if (c.last_action.amount == bb(0))
        noamt.push_back(children[i]);
      else
        wamt.push_back(children[i]);
    }

    //std::cout << noamt.size() << std::endl;
    //std::cout << wamt.size() << std::endl << "\n\n";

    // make ratios from the wamts
    vector<double> ratios;
    for (int i = 0; i < wamt.size(); ++i) {
      FContext c = wamt[i]->context();
      ratios.push_back( wamt[i]->ev() / c.last_action.amount.getAsDouble() );
      //std::cout << ratios[i] << std::endl;
    }

    // find max ratio action and check if its > threshold
    auto max = std::max_element(ratios.begin(), ratios.end());
    int index = std::distance(ratios.begin(), max);
    //std::cout << "\n\n" << *max << std::endl;
    //std::cout << index << std::endl;

    // check if max is bigger than threshold
    if( *max >= threshold )
        return wamt[index];

    // choose a zero amount action because threshold was not reached.
    return noamt.back();
  }
};
}

#endif	/* BETAMT_EV_RATIO_SELECTOR_H */

