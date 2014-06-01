#include <algorithm>
#include "bucket_collection.hpp"

namespace Freedom5 {

BucketCollection::BucketCollection(int _nb_buckets)
    : nb_buckets(_nb_buckets), buckets(nb_buckets) {}

BucketCollection::BucketCollection(vector<Bucket> _buckets)
    : buckets(_buckets), nb_buckets(_buckets.size()) {}

BucketCollection::Bucket &BucketCollection::operator[](unsigned int i) { return buckets[i]; }

const BucketCollection::Bucket &BucketCollection::operator[](unsigned int i) const {
  return buckets[i];
}

int BucketCollection::get_bucket_index_including_nb_hands(int nb_hands,
                                                          int bucket_offset) {
  int start = 0 + bucket_offset;
  int sum = 0;
  for (int i = start; i < buckets.size(); ++i) {
    sum += buckets[i].size();
    if (sum >= nb_hands)
      return i;
  }
  // the number of demanded hands is greater than the
  // number of hands in the buckets.
  return -1;
}

vector<BucketHand> BucketCollection::hand_bucket_range(int lower_bound,
                                                       int upper_bound) {
  vector<BucketHand> range;
  for (int i = upper_bound; i <= lower_bound; ++i) {
    range.insert(range.end(), buckets[i].begin(), buckets[i].end());
  }
  return range;
}
  
BucketCollection BucketCollection::bucket_range(int lower_bound, int upper_bound){
  BucketCollection range(0);
  for (int i = upper_bound; i <= lower_bound; ++i) {
    range.buckets.push_back(buckets[i]);
  }
  return range;
}

int BucketCollection::count_hands() const {
  int sum = 0;
  for (auto bucket : buckets)
    sum += bucket.size();
  return sum;
}

BucketCollection BucketCollection::remove_empty_buckets() {
  vector<Bucket> newc;
  for (unsigned i = 0; i < buckets.size(); ++i)
    if (!buckets[i].empty())
      newc.push_back(buckets[i]);
  return BucketCollection(newc);
}
};
