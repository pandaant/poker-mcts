#ifndef BUCKET_COLLECTION_H
#define BUCKET_COLLECTION_H

#include <vector>
#include <poker/hand.hpp>
#include "bucket_hand.hpp"

namespace freedom {

using std::vector;
using poker::Hand;

// ----------------------------------------------------------------------
/// @brief   wrapper for a list of buckets.
// ----------------------------------------------------------------------
class BucketCollection {
public:
  typedef vector<BucketHand> Bucket;

  explicit BucketCollection(const unsigned &nb_buckets_);
  explicit BucketCollection(const vector<Bucket> &buckets_);

  Bucket &operator[](unsigned int i);
  const Bucket &operator[](unsigned int i) const;

  // ----------------------------------------------------------------------
  /// @brief   sum nbHands top down from the buckets and return the bucket
  ///          index that includes the number of hands. if an offset is
  ///          used, the counting starts from nbBuckets-offset to count hands.
  ///
  /// @param nb_hands to include
  /// @param bucket_offset buckets to skip
  ///
  /// @return upper bound of bucket range, lower range is bucket_offset
  // ----------------------------------------------------------------------
  int
  get_bucket_index_including_nb_hands(const unsigned &nb_hands,
                                      const unsigned &bucket_offset = 0) const;

  // ----------------------------------------------------------------------
  /// @brief   returns all hands in bucket i where
  ///          i <= upperbound && i > lower_bound. Does not check boundaries.
  ///
  /// @param lower_bound index of lowest bucket
  /// @param upper_bound index of highest bucket to select
  ///
  /// @return list of hands that was contained in selected buckets.
  // ----------------------------------------------------------------------
  vector<BucketHand> hand_bucket_range(const unsigned &lower_bound,
                                       const unsigned &upper_bound) const;

  // ----------------------------------------------------------------------
  /// @brief   returns all buckets with i where i <= upperbound && i >
  /// lower_bound.
  ///          Does not check boundaries, careful.
  ///
  /// @param lower_bound index of lowest bucket
  /// @param upper_bound index of highest bucket to select
  ///
  /// @return list of buckets that was beween high and lower bound.
  // ----------------------------------------------------------------------
  BucketCollection bucket_range(const unsigned &lower_bound,
                                const unsigned &upper_bound) const;

  // ----------------------------------------------------------------------
  /// @brief   returns number of hands stored in all buckets.
  ///
  /// @return number of hands
  // ----------------------------------------------------------------------
  unsigned nb_hands() const;

  // ----------------------------------------------------------------------
  /// @brief   returns a list of buckets with all empty buckets removed.
  ///
  /// @return new bucketcollection without empty buckets
  // ----------------------------------------------------------------------
  BucketCollection remove_empty_buckets() const;

  // ----------------------------------------------------------------------
  /// @brief   returns number of buckets contained in the collection
  ///
  /// @return number of buckets
  // ----------------------------------------------------------------------
  unsigned nb_buckets() const;

  // ----------------------------------------------------------------------
  /// @brief   reverses the order of interal bucket vector
  // ----------------------------------------------------------------------
  void reverse();

  // ----------------------------------------------------------------------
  /// @brief   returns all hands in all buckets
  ///
  /// @return hands
  // ----------------------------------------------------------------------
  vector<BucketHand> hands() const;

private:
  vector<Bucket> buckets;
};
}

#endif
