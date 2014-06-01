/* 
 * File:   ehs2_range_predictor.h
 * Author: batman
 *
 * Created on February 26, 2014, 11:24 PM
 */

#ifndef EHS2_RANGE_PREDICTOR_H
#define	EHS2_RANGE_PREDICTOR_H

#include "range_predictor.hpp"

namespace Freedom5 {
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
class EHS2RangePredictor : public RangePredictor{
public:
  /**
   * Constructor
   * @look @ class RangePredictor
   */
  EHS2RangePredictor(ECalc *_calc, int _nb_samples,
                 Threshold _thresholds = Threshold(2, 2),
                 vector<int> _nb_buckets = vector<int>(4, 20),
                 int _threshold_min_hands = 5,
                 double _zerop_rc = 0.20);
  
  /**
   * calculates equity (pwin+ptie) for the first handlist.
   * squares the equity.
   * @param lists
   * @param board
   * @param deadcards
   * @return
   */
  double equity_first(vector<Handlist *> lists, vector<unsigned> board,
                      vector<unsigned> deadcards, ECalc *c);
};
};

#endif	/* EHS2_RANGE_PREDICTOR_H */

