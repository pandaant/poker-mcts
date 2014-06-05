#ifndef PHASE_HISTOGRAMM_H
#define PHASE_HISTOGRAMM_H

#include "round_histogramm.hpp"

namespace freedom {

class PhaseHistogramm {
public:
  vector<RoundHistogramm> round_data;

  explicit PhaseHistogramm(unsigned betting_rounds = 5);
  PhaseHistogramm(const vector<RoundHistogramm> &round_data_);
  PhaseHistogramm(const PhaseHistogramm &oph);
  PhaseHistogramm(const Value &data);
};
}

#endif
