#include "ehs2_range_predictor.hpp"

namespace freedom {
EHS2RangePredictor::EHS2RangePredictor(ECalc *_calc,
                                       const unsigned &nb_samples_,
                                       const Threshold &thresholds_,
                                       const vector<unsigned> &nb_buckets_,
                                       const unsigned &threshold_min_hands_,
                                       const double &zerop_rc_)
    : RangePredictor(calc, nb_samples_, thresholds_, nb_buckets_,
                     threshold_min_hands_) {}

double EHS2RangePredictor::equity_first(const Handlist::collection_t &lists,
                                        const vector<unsigned> &board,
                                        const vector<unsigned> &deadcards,
                                        ECalc *c) {
  ecalc::result_collection results;
  results = c->evaluate(lists, board, deadcards, nb_samples);
  double equity = results[0].pwin_tie();
  return equity * equity;
}
}
