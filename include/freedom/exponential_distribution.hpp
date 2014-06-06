#ifndef EXPONENTIAL_DISTRIBUTION_H
#define EXPONENTIAL_DISTRIBUTION_H

#include <cmath>
#include <math.h>

namespace freedom {

// ----------------------------------------------------------------------
/// @brief   encapsulates a exponential function with scaling parameter
///          lambda.
///          https://en.wikipedia.org/wiki/Exponential_distribution
// ----------------------------------------------------------------------
class ExponentialDistribution {
public:
  explicit ExponentialDistribution(const double &lambda_) : lambda(lambda_) {}

  // ----------------------------------------------------------------------
  /// @brief   gets the function value of point x
  ///
  /// @param x point to get value for
  ///
  /// @return f(x)
  // ----------------------------------------------------------------------
  double operator()(const double &x) const {
    return lambda * exp(-(lambda * x));
  }

private:
  const double Euler = std::exp(1.0);
  double lambda;
};
}

#endif
