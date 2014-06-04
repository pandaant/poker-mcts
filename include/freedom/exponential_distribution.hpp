/* 
 * File:   exponential_distribution.h
 * Author: batman
 *
 * Created on April 30, 2014, 12:48 PM
 */

#ifndef EXPONENTIAL_DISTRIBUTION_H
#define	EXPONENTIAL_DISTRIBUTION_H

#include <cmath>
#include <math.h>

namespace freedom {

//  https://en.wikipedia.org/wiki/Exponential_distribution
class ExponentialDistribution {
  const double Euler = std::exp(1.0);

  double lambda;
public:
  ExponentialDistribution(double _lambda) : lambda(_lambda) {}
  double operator()(double x) { return lambda * exp(-(lambda * x)); }
};
}

#endif	/* EXPONENTIAL_DISTRIBUTION_H */
