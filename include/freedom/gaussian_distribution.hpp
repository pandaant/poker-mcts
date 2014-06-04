/*
 * File:   gaussian_distribution.h
 * Author: batman
 *
 * Created on April 30, 2014, 12:47 PM
 */

#ifndef GAUSSIAN_DISTRIBUTION_H
#define GAUSSIAN_DISTRIBUTION_H

#include <cmath>
#include <math.h>

namespace freedom {

class GaussianDistribution {
  const double Euler = std::exp(1.0);
  const double PI = M_PI;
  const double sqrt2PI = sqrt(2 * PI);

  // mean / expectation mü
  double u;
  // std deviation
  double o;

  double base;

public:
  GaussianDistribution(double _u, double _o) : u(_u), o(_o), base(1.0 / (_o * sqrt2PI)) {}
  double operator()(double x) {
    double exponent = -(((x - u) * (x - u)) / (2 * (o * o)));
    return base * exp(exponent);
  }
};
}

#endif /* GAUSSIAN_DISTRIBUTION_H */

