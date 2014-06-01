#include "exponential_bucketizer.hpp"
#include <cmath>
#include <algorithm>

namespace Freedom5{
BucketCollection
ExponentialBucketizer::map_hands(int nb_buckets,
                              vector<BucketHand> &_hands) const {
  BucketCollection bcollection(nb_buckets);

  double prob_sum = 0.0;
  vector<int> bucket_hands(nb_buckets); 
  vector<double> bucket_probs(nb_buckets);

  // calc % of hands per bucket
  for( int i = 1; i <= nb_buckets; ++i )
      prob_sum += (1.0 / (i*i)  );

  // calc probability for each bucket
  for( int i = 1; i <= nb_buckets; ++i )
      bucket_probs[i-1] = (1.0 / (i*i)) / prob_sum;

  // map probability to a number of hands per bucket
  for( int i = 0; i < nb_buckets; ++i )
    bucket_hands[i] = std::ceil( bucket_probs[i] * _hands.size() ); 

  // sort in ( from worst to best hand )
  int curr_hand_index = _hands.size() - 1;

  for (int i = 0; i < nb_buckets ; ++i) {
      int take = bucket_hands[i];
      for (int j = 0; (j < take) && (curr_hand_index >= 0); ++j) {
        bcollection[i].push_back(_hands[curr_hand_index]);
        --curr_hand_index;
      }
      // TODO sort in in reverse directly above?
      std::reverse(bcollection[i].begin(), bcollection[i].end());
  }
  std::reverse(bcollection.buckets.begin(), bcollection.buckets.end());
  return bcollection;
}
};
