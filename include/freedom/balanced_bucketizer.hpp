/*
 * File:   balanced_bucketizer.h
 * Author: batman
 *
 * Created on August 14, 2013, 3:43 PM
 */

#ifndef BALANCED_BUCKETIZER_H
#define BALANCED_BUCKETIZER_H

#include "bucketizer.hpp"

namespace Freedom5 {

class BalancedBucketizer : public Bucketizer {
public:
  virtual BucketCollection map_hands(int nb_buckets,
                                     vector<BucketHand> &_hands) const;
};
};

#endif /* BALANCED_BUCKETIZER_H */

