#include "phase_histogramm.hpp"

namespace freedom {

PhaseHistogramm::PhaseHistogramm(unsigned betting_rounds)
    : round_data(vector<RoundHistogramm>(betting_rounds)) {}

PhaseHistogramm::PhaseHistogramm(const PhaseHistogramm &oph)
    : round_data(oph.round_data) {}

PhaseHistogramm::PhaseHistogramm(const vector<RoundHistogramm> &round_data_)
    : round_data(round_data_) {}

PhaseHistogramm::PhaseHistogramm(const Value &data) {
  for (SizeType i = 0; i < data.Size(); ++i)
    round_data.push_back(RoundHistogramm(data[i]));
}
}
