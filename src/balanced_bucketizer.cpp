#include <cmath>
#include "balanced_bucketizer.hpp"

namespace Freedom5 {

BucketCollection
BalancedBucketizer::map_hands(int nb_buckets,
                              vector<BucketHand> &_hands) const {
  BucketCollection bcollection(nb_buckets);

  double div = _hands.size() < nb_buckets ? _hands.size() : nb_buckets;
  int hands_per_bucket = ceil(_hands.size() / div);
  int s_hands = _hands.size() - 1;
  int last_index = 0;
  for (unsigned b = 0; b < nb_buckets; ++b) {
    for (int j = 0; j < hands_per_bucket; ++j) {
      if (last_index > s_hands)
        break;
      bcollection.buckets[b].push_back(_hands[last_index]);
      ++last_index;
    }
  }

  return bcollection;
}
};
