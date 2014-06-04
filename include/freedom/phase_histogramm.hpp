/*
 * File:   phase_histogramm.h
 * Author: batman
 *
 * Created on September 4, 2013, 10:59 AM
 */

#ifndef PHASE_HISTOGRAMM_H
#define PHASE_HISTOGRAMM_H

#include "round_histogramm.hpp"

namespace freedom {

class PhaseHistogramm {
public:
  vector<RoundHistogramm> round_data;

  PhaseHistogramm(int betting_rounds = 5):
    round_data(vector<RoundHistogramm>(betting_rounds)){}
  PhaseHistogramm(vector<RoundHistogramm> _round_data);
  PhaseHistogramm(const PhaseHistogramm &oph);
  PhaseHistogramm(const Value &data);
};
};

#endif /* PHASE_HISTOGRAMM_H */

