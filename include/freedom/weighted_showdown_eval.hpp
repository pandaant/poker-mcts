#ifndef WEIGHTED_SHOWDOWN_EVAL_H
#define WEIGHTED_SHOWDOWN_EVAL_H

#include <map>
#include <random>
#include "showdown_eval.hpp"
#include "gaussian_distribution.hpp"
#include "exponential_distribution.hpp"
#include "weighted_bucket_handlist.hpp"

namespace freedom {
class WeightedShowdownEval : public ShowdownEval {
    std::map<string, WeightedBucketHandlist* > cache;
public:
  WeightedShowdownEval(ECalc *_ecalc, int _samples);
  virtual ~WeightedShowdownEval();
  virtual double simulate(const FContext &context);
  virtual vector<double> modify_range(const vector<double>& bucket_probabilities, const ActionSequence& seq) const;

  /**
   * applies the exponential or gauss function to 
   * the index of every element in the vector and 
   * adds it to the current index's value
   */
  virtual void apply_exp(vector<double> &vec, const double &lambda = 1, const double &multiplicator = 1) const;
  virtual void apply_gauss(vector<double> &vec, const double &mean = 0,
                           const double &std_dev = 1) const;

  bool is_cached(const string &key) const{
    return cache.find(key) != cache.end();
  }
};
}

#endif
