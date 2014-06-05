#include <cmath>
#include "balanced_bucketizer.hpp"

namespace freedom {

BucketCollection
BalancedBucketizer::map_hands(const unsigned &nb_buckets,
                              const vector<BucketHand> &hands_) const {
  BucketCollection bcollection(nb_buckets);

  double div = hands_.size() < nb_buckets ? hands_.size() : nb_buckets;
  int hands_per_bucket = ceil(hands_.size() / div);
  int shands_ = hands_.size() - 1;
  int last_index = 0;
  for (unsigned b = 0; b < nb_buckets; ++b) {
    for (int j = 0; j < hands_per_bucket; ++j) {
      if (last_index > shands_)
        break;
      bcollection[b].push_back(hands_[last_index]);
      ++last_index;
    }
  }

  return bcollection;
}
};
