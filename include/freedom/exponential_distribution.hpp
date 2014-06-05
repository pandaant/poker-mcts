#ifndef EXPONENTIAL_DISTRIBUTION_H
#define EXPONENTIAL_DISTRIBUTION_H

#include <cmath>
#include <math.h>

namespace freedom {

//  https://en.wikipedia.org/wiki/Exponential_distribution
class ExponentialDistribution {
public:
  explicit ExponentialDistribution(const double &lambda_) : lambda(lambda_) {}
  double operator()(const double &x) const {
    return lambda * exp(-(lambda * x));
  }

private:
  const double Euler = std::exp(1.0);
  double lambda;
};
}

#endif
