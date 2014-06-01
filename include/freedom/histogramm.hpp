/*
 * File:   histogramm.h
 * Author: batman
 *
 * Created on August 23, 2013, 6:58 PM
 */

#ifndef HISTOGRAMM_H
#define HISTOGRAMM_H

#include <tr1/tuple>
#include "model.hpp"
#include "phase_histogramm.hpp"

namespace Freedom5 {
using std::tr1::get;

class Histogramm : public Model {
public:
  vector<PhaseHistogramm> histogramm;

  Histogramm();
  Histogramm(string _name, vector<PhaseHistogramm> _histogramm);
  Histogramm(const Histogramm &oh);
  Histogramm(const Value &data);

  virtual tuple<double, double, double>
  getFoldCallRaiseProbabilities(const FContext &gs, int index_seat) const;
  virtual tuple<double, double> getCheckBetProbabilities(const FContext &gs,
                                                         int index_seat) const;
  virtual tuple<double, double> getFoldCallProbabilities(const FContext &gs,
                                                         int index_seat) const;
  virtual double get_action_probability(Action action,
                                        PhaseType::Enum phase, int betting_round) const;
  virtual Model* clone(){
    return new Histogramm(*this);
  }
};
};

#endif /* HISTOGRAMM_H */

