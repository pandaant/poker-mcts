/*
 * File:   bucketizer.h
 * Author: batman
 *
 * Created on August 14, 2013, 3:40 PM
 */

#ifndef BUCKETIZER_H
#define BUCKETIZER_H

#include "bucket_collection.hpp"

namespace Freedom5 {
class Bucketizer {
public:
  /**
    * Strong hands are in low index buckets, bad hand in big index buckets.
    * example: AA is in bucket 0, 72o is in bucket n
    */
  virtual BucketCollection map_hands(int nb_buckets,
                                     vector<BucketHand> &_hands) const = 0;
};
};

#endif /* BUCKETIZER_H */

