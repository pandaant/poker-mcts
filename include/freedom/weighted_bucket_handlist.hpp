#ifndef WEIGHTED_BUCKET_HANDLIST_H
#define WEIGHTED_BUCKET_HANDLIST_H

#include <random>
#include <algorithm>
#include <ecalc/handlist.hpp>
#include <ecalc/array_handlist.hpp>
#include <poker/action_sequence.hpp>
#include "bucket_collection.hpp"

namespace freedom {

using poker::ActionSequence;
using ecalc::combination;

class WeightedBucketHandlist : public ecalc::Handlist {
public:
  vector<double> weights;
  std::discrete_distribution<int> dist;
  ActionSequence range_sequence;
  vector<ecalc::ArrayHandlist> buckets;

  WeightedBucketHandlist(const BucketCollection &buckets_,
                         const vector<double> &weights_,
                         const ActionSequence &range_sequence_);

  WeightedBucketHandlist(const BucketCollection &buckets_,
                         const vector<double> &weights_);

  WeightedBucketHandlist(const WeightedBucketHandlist &wbh,
                         const vector<double> &new_weights);

  virtual combination get_hand(ecalc::XOrShiftGenerator &nb_gen,
                                      ecalc::bitset &deck);

  // returns the sum of all weights each bucket has
  virtual unsigned weight_sum() const{
    unsigned sum = 0;
    for (unsigned i = 0; i < weights.size(); ++i)
      sum += weights[i];
    return sum;
  }

private:
  // ----------------------------------------------------------------------
  /// @brief   converts bucketcollection to a list of arrayhandlists that
  ///          contain the same hands.
  ///
  /// @param collection of handlists
  // ----------------------------------------------------------------------
  vector<ecalc::ArrayHandlist> convert_buckets(const BucketCollection &collection) const;

  const unsigned MAX_TRIES = 25;
};
}

#endif
