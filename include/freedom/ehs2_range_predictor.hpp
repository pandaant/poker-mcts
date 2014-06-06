#ifndef EHS2_RANGE_PREDICTOR_H
#define EHS2_RANGE_PREDICTOR_H

#include "range_predictor.hpp"

namespace freedom {
using std::vector;
using ecalc::ECalc;
using ecalc::Handlist;
using poker::Hand;
using poker::Action;
using poker::ActionSequence;

/**
 * this range predictor uses
 * expected handstregth squared.
 *
 * this should result in a better balance
 * between strong hand and high potential hands.
 */
class EHS2RangePredictor : public RangePredictor {
public:
  /**
   * Constructor
   * @look @ class RangePredictor
   */
  EHS2RangePredictor(ECalc *calc_, const unsigned &nb_samples_,
                     const Threshold &thresholds_ = Threshold(2, 2),
                     const vector<unsigned> &nb_buckets_ = vector<unsigned>(4,
                                                                            20),
                     const unsigned &threshold_min_hands_ = 5,
                     const double &zerop_rc_ = 0.20);

  /**
   * calculates equity (pwin+ptie) for the first handlist.
   * squares the equity.
   * @param lists
   * @param board
   * @param deadcards
   * @return
   */
  double equity_first(const Handlist::collection_t &lists,
                      const cards &board,
                      const cards &deadcards, ECalc *c) const;
};
}

#endif
