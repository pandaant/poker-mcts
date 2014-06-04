#ifndef BALANCED_BUCKETIZER_H
#define BALANCED_BUCKETIZER_H

#include "ibucketizer.hpp"

namespace freedom {

class BalancedBucketizer : public IBucketizer {
public:
  virtual BucketCollection map_hands(unsigned nb_buckets,
                                     vector<BucketHand> &hands_) const;
};
}

#endif
