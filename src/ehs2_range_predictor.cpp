#include "ehs2_range_predictor.hpp"

namespace freedom {
EHS2RangePredictor::EHS2RangePredictor(ECalc *_calc, int _nb_samples,
                                       Threshold _thresholds,
                                       vector<int> _nb_buckets,
                                       int _threshold_min_hands,
                                       double _zerop_rc)
    : RangePredictor(_calc, _nb_samples, _thresholds, _nb_buckets,
                     _threshold_min_hands) {}

double EHS2RangePredictor::equity_first(Handlist::collection_t lists,
                                        vector<unsigned> board,
                                        vector<unsigned> deadcards, ECalc *c) {
    ecalc::result_collection results;
  results = c->evaluate(lists, board, deadcards, nb_samples);
  double equity = results[0].pwin_tie();
  return equity * equity;
}
};
