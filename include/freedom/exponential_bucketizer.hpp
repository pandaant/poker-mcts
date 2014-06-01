/* 
 * File:   exponential_bucketizer.h
 * Author: batman
 *
 * Created on March 25, 2014, 9:16 PM
 */

#ifndef EXPONENTIAL_BUCKETIZER_H
#define	EXPONENTIAL_BUCKETIZER_H

#include "bucketizer.hpp"

namespace Freedom5{
class ExponentialBucketizer {
public:
  virtual BucketCollection map_hands(int nb_buckets,
                                     vector<BucketHand> &_hands) const;
};
};

#endif	/* EXPONENTIAL_BUCKETIZER_H */

