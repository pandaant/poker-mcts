#ifndef HISTOGRAMM_H
#define HISTOGRAMM_H

#include <tuple>
#include "imodel.hpp"
#include "phase_histogramm.hpp"

namespace freedom {
using std::get;

// ----------------------------------------------------------------------
/// @brief   represents a model that uses underlying history data to
///          calculate action probabilities
// ----------------------------------------------------------------------
class Histogramm : public IModel {
public:
  /// stores the underlying data
  vector<PhaseHistogramm> histogramm;

  explicit Histogramm(vector<PhaseHistogramm> _histogramm);
  Histogramm(const Histogramm &oh);
  Histogramm(const Value &data);
  virtual ~Histogramm();

  virtual tuple<double, double, double>
  getFoldCallRaiseProbabilities(const FContext &gs) const;
  virtual tuple<double, double>
  getCheckBetProbabilities(const FContext &gs) const;

  virtual tuple<double, double>
  getFoldCallProbabilities(const FContext &gs) const;

  virtual double get_action_probability(const Action &action,
                                        const PhaseType::Enum &phase,
                                        const unsigned &betting_round) const;

  virtual IModel *clone() const;
};
}

#endif
