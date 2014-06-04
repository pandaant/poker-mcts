#ifndef GAUSSIAN_DISTRIBUTION_H
#define GAUSSIAN_DISTRIBUTION_H

#include <cmath>
#include <math.h>

namespace freedom {

// ----------------------------------------------------------------------
/// @brief   encapsulates a gaussian distribution with parameters u and o
// ----------------------------------------------------------------------
class GaussianDistribution {
  const double Euler = std::exp(1.0);
  const double sqrt2PI = sqrt(2 * M_PI);

  /// mean / expectation mü
  double u;
  /// std deviation
  double o;

  double base;

public:
  // ----------------------------------------------------------------------
  /// @brief   creates a new distribution
  ///
  /// @param _u the mean of the distribution
  /// @param _o the std deviation of the d.
  // ----------------------------------------------------------------------
  GaussianDistribution(double u_, double o_)
      : u(u_), o(o_), base(1.0 / (o_ * sqrt2PI)) {}

  // ----------------------------------------------------------------------
  /// @brief   gets the value of the distribution at point x.
  ///
  /// @param x point to retrive value for
  ///
  /// @return number between 0-1
  // ----------------------------------------------------------------------
  double operator()(double x) {
    double exponent = -(((x - u) * (x - u)) / (2 * (o * o)));
    return base * exp(exponent);
  }
};
}

#endif
