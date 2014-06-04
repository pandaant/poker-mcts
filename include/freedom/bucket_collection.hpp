/*
 * File:   bucket_collection.h
 * Author: batman
 *
 * Created on August 13, 2013, 11:40 PM
 */

#ifndef BUCKET_COLLECTION_H
#define BUCKET_COLLECTION_H

#include <vector>
#include <poker/hand.hpp>
#include "bucket_hand.hpp"

namespace freedom {
using std::vector;
using poker::Hand;

class BucketCollection {
public:
  typedef vector<BucketHand> Bucket;

  int nb_buckets;
  vector<Bucket> buckets;

  BucketCollection(int _nb_buckets);
  BucketCollection(vector<Bucket> _buckets);

  Bucket &operator[](unsigned int i);
  const Bucket &operator[](unsigned int i) const;

  /**
   * sum nbHands hands top down from the buckets and returns the bucket
   * index that includes the number of hands. if an offset is used, the counting
   * starts from nbBuckets-offset to count hands.
   **/
  int get_bucket_index_including_nb_hands(int nb_hands, int bucket_offset = 0);

  /**
   * returns all hands in bucket i where i <= upperbound && i > lower_bound.
   * Does not check boundaries, careful.
   * @param lower_bound
   * @param upper_bound
   * @return
   */
  vector<BucketHand> hand_bucket_range(int lower_bound, int upper_bound);

  /**
   * returns all buckets with i where i <= upperbound && i > lower_bound.
   * Does not check boundaries, careful.
   * @param lower_bound
   * @param upper_bound
   * @return
   */
  BucketCollection bucket_range(int lower_bound, int upper_bound);

  // returns the number of hands stored in all buckets combinded.
  int count_hands() const;

  // erase all buckets that contain no hands and return
  // the new collection
  BucketCollection remove_empty_buckets();
};
};

#endif /* BUCKET_COLLECTION_H */

