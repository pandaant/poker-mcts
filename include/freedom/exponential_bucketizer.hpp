#ifndef EXPONENTIAL_BUCKETIZER_H
#define	EXPONENTIAL_BUCKETIZER_H

#include "bucket_collection.hpp"

namespace freedom{
class ExponentialBucketizer {
public:
  // ----------------------------------------------------------------------
  /// @brief   maps a number of hands to a number of buckets according
  ///          to an balances distribution (same nb of hands in each bucket)
  ///          Strong hands are in low index buckets, bad hand in big index
  /// buckets.
  ///          example: AA is in bucket 0, 72o is in bucket n
  ///
  /// @param nb_buckets number of buckets to create
  /// @param hands_ hands to map
  ///
  /// @return mapped hands in a bucketcollection
  // ----------------------------------------------------------------------
  virtual BucketCollection map_hands(const unsigned &nb_buckets,
                                     const vector<BucketHand> &hands_) const;
};
}

#endif
