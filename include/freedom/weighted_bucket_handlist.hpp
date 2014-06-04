/*
 * File:   array_handlist.h
 * Author: batman
 *
 * Created on April 28, 2014, 10:19 AM
 */

#ifndef WEIGHTED_BUCKET_HANDLIST_H
#define WEIGHTED_BUCKET_HANDLIST_H

#include <algorithm>
#include <poker/action_sequence.hpp>
#include <ecalc/handlist.hpp>
#include <ecalc/array_handlist.hpp>
#include "bucket_collection.hpp"

namespace freedom {

using poker::ActionSequence;

class WeightedBucketHandlist : public ecalc::Handlist {
  const unsigned MAX_TRIES = 25;

  std::discrete_distribution<int> dist;

public:
  // stores a sequence that led to this range
  // * optional *
  ActionSequence range_sequence;
  vector<ecalc::ArrayHandlist> buckets;
  vector<double> weights;

  WeightedBucketHandlist(BucketCollection _buckets, vector<double> _weights,
                         ActionSequence _range_sequence)
      : weights(_weights), dist(_weights.begin(), _weights.end()),
        range_sequence(_range_sequence) {
    for (unsigned i = 0; i < _buckets.buckets.size(); ++i) {
      vector<poker::Hand> range;
      for (auto bhand : _buckets.buckets[i])
        range.push_back(static_cast<Hand>(bhand));
      buckets.push_back(ecalc::ArrayHandlist(range));
    }
  }

  WeightedBucketHandlist(BucketCollection _buckets, vector<double> _weights)
      : weights(_weights), dist(_weights.begin(), _weights.end()),
        range_sequence() {
    for (unsigned i = 0; i < _buckets.buckets.size(); ++i) {
      vector<poker::Hand> range;
      for (auto bhand : _buckets.buckets[i])
        range.push_back(static_cast<Hand>(bhand));
      buckets.push_back(ecalc::ArrayHandlist(range));
    }
  }

  WeightedBucketHandlist(const WeightedBucketHandlist &wbh,
                         vector<double> new_weights)
      : weights(new_weights), dist(new_weights.begin(), new_weights.end()),
        range_sequence(wbh.range_sequence), buckets(wbh.buckets){
  }

  
  virtual ecalc::combination get_hand(ecalc::XOrShiftGenerator &nb_gen, ecalc::bitset &deck);

  // returns the sum of all weights each bucket has
  virtual unsigned weight_sum(){
    unsigned sum = 0;
    for(unsigned i = 0; i < weights.size(); ++i)
        sum += weights[i];
    return sum;
  }
};
};

#endif /* WEIGHTED_BUCKET_HANDLIST_H */

