#include "phase_histogramm.hpp"

namespace freedom {

PhaseHistogramm::PhaseHistogramm(const PhaseHistogramm &oph)
    : round_data(oph.round_data) {}

PhaseHistogramm::PhaseHistogramm(vector<RoundHistogramm> _round_data)
    : round_data(_round_data) {}

PhaseHistogramm::PhaseHistogramm(const Value &data) {
  for (SizeType i = 0; i < data.Size(); ++i)
    round_data.push_back(RoundHistogramm(data[i]));
}
};
