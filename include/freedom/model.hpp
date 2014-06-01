/*
 * File:   model.h
 * Author: batman
 *
 * Created on August 14, 2013, 3:21 PM
 */

#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <tr1/tuple>
#include "fcontext.hpp"

namespace Freedom5 {

using std::string;
using std::tuple;

class Model {
public:
  string name;

  Model() {}
  Model(string _name) : name(_name) {}
  virtual ~Model() {}

  virtual tuple<double, double, double>
  getFoldCallRaiseProbabilities(const FContext &gs, int index_seat) const = 0;
  virtual tuple<double, double>
  getCheckBetProbabilities(const FContext &gs, int index_seat) const = 0;
  virtual tuple<double, double>
  getFoldCallProbabilities(const FContext &gs, int index_seat) const = 0;
  virtual double get_action_probability(Action action,
                                        PhaseType::Enum phase, int betting_round) const = 0;
  virtual Model* clone() = 0;
};
};

#endif /* MODEL_H */

