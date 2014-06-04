#ifndef BUCKETIZER_H
#define BUCKETIZER_H

#include "bucket_collection.hpp"

namespace freedom {

// ----------------------------------------------------------------------
/// @brief   interface for bucketizers
// ----------------------------------------------------------------------
class IBucketizer {
public:
  // ----------------------------------------------------------------------
  /// @brief   maps a number of hands to a number of buckets according
  ///          to some metric.
  ///          Strong hands are in low index buckets, bad hand in big index
  /// buckets.
  ///          example: AA is in bucket 0, 72o is in bucket n
  ///
  /// @param nb_buckets number of buckets to create
  /// @param hands_ hands to map
  ///
  /// @return mapped hands in a bucketcollection
  // ----------------------------------------------------------------------
  virtual BucketCollection map_hands(unsigned nb_buckets,
                                     vector<BucketHand> &hands_) const = 0;
};
}

#endif
