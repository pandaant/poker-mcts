/*
 * File:   freedom.h
 * Author: batman
 *
 * Created on August 6, 2013, 12:27 PM
 */

#ifndef FREEDOM_H
#define FREEDOM_H

#include <memory>
#include <mcts/mcts.hpp>
#include <ecalc/ecalc.hpp>
#include <ecalc/handranks.hpp>
#include "range_predictor.hpp"
#include "decision_node.hpp"

namespace freedom {

using ecalc::ECalc;
using ecalc::Handranks;
using mcts::MCTS;
using std::shared_ptr;

class Freedom {
  Handranks* handranks;

public:
  typedef shared_ptr<Freedom> ptr;
  ECalc *ecalc;
  RangePredictor *pr;

  Freedom(Handranks* handranks);
  virtual ~Freedom();

  // calculate a range for every player
  void generate_handranges(FContext &context, FConfig *config);
};
}

#endif /* freedom_H */

