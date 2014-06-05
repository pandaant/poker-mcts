#include <cmath>
#include <algorithm>
#include "exponential_bucketizer.hpp"

namespace freedom{
BucketCollection
ExponentialBucketizer::map_hands(const unsigned &nb_buckets,
                              const vector<BucketHand> &hands_) const {
  BucketCollection bcollection(nb_buckets);

  double prob_sum = 0.0;
  vector<int> buckethands_(nb_buckets); 
  vector<double> bucket_probs(nb_buckets);

  // calc % of hands per bucket
  for( int i = 1; i <= nb_buckets; ++i )
      prob_sum += (1.0 / (i*i)  );

  // calc probability for each bucket
  for( int i = 1; i <= nb_buckets; ++i )
      bucket_probs[i-1] = (1.0 / (i*i)) / prob_sum;

  // map probability to a number of hands per bucket
  for( int i = 0; i < nb_buckets; ++i )
    buckethands_[i] = std::ceil( bucket_probs[i] * hands_.size() ); 

  // sort in ( from worst to best hand )
  int curr_hand_index = hands_.size() - 1;

  for (int i = 0; i < nb_buckets ; ++i) {
      int take = buckethands_[i];
      for (int j = 0; (j < take) && (curr_hand_index >= 0); ++j) {
        bcollection[i].push_back(hands_[curr_hand_index]);
        --curr_hand_index;
      }
      // TODO sort in in reverse directly above?
      std::reverse(bcollection[i].begin(), bcollection[i].end());
  }
  bcollection.reverse();
  return bcollection;
}
};
