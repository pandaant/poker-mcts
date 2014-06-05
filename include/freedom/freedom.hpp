#ifndef FREEDOM_H
#define FREEDOM_H

#include <memory>
#include <mcts/mcts.hpp>
#include <ecalc/ecalc.hpp>
#include <ecalc/handranks.hpp>
#include "range_predictor.hpp"
#include "decision_node.hpp"

namespace freedom {

using mcts::MCTS;
using ecalc::ECalc;
using ecalc::Handranks;
using std::shared_ptr;

class Freedom {

public:
  explicit Freedom(Handranks *handranks_);
  virtual ~Freedom();

  // calculate a range for every player
  void generate_handranges(FContext &context, FConfig *config);

private:
  ECalc *ecalc;
  RangePredictor *range_predictor;
};
}

#endif
