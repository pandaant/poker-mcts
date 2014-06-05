#ifndef BALANCED_BUCKETIZER_H
#define BALANCED_BUCKETIZER_H

#include "bucket_collection.hpp"

namespace freedom {

class BalancedBucketizer{
public:
  // ----------------------------------------------------------------------
  /// @brief   maps a number of hands to a number of buckets according
  ///          to an balanced distribution. every buckets same nb hands.
  ///          Strong hands are in low index buckets, bad hand in big index
  ///          buckets.
  ///          example: AA is in bucket 0, 72o is in bucket n
  ///
  /// @param nb_buckets number of buckets to create
  /// @param hands_ hands to map
  ///
  /// @return mapped hands in a bucketcollection
  // ----------------------------------------------------------------------
  BucketCollection map_hands(const unsigned &nb_buckets,
                                     const vector<BucketHand> &hands_) const;
};
}

#endif
