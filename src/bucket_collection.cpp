#include <algorithm>
#include "bucket_collection.hpp"

namespace freedom {

BucketCollection::BucketCollection(const unsigned &nb_buckets)
    : buckets(nb_buckets) {}

BucketCollection::BucketCollection(const vector<Bucket> &buckets_)
    : buckets(buckets_) {}

BucketCollection::Bucket &BucketCollection::operator[](unsigned int i) {
  return buckets[i];
}

const BucketCollection::Bucket &BucketCollection::
operator[](unsigned int i) const {
  return buckets[i];
}

int BucketCollection::get_bucket_index_including_nb_hands(
    const unsigned &nb_hands, const unsigned &bucket_offset) const {
  unsigned sum = 0;
  for (int i = bucket_offset; i < buckets.size(); ++i) {
    sum += buckets[i].size();
    if (sum >= nb_hands)
      return i;
  }
  // the number of demanded hands is greater than the
  // number of hands in the buckets. return lowest bucket
  return buckets.size() - 1;
}

vector<BucketHand>
BucketCollection::hand_bucket_range(const unsigned &lower_bound,
                                    const unsigned &upper_bound) const {
  vector<BucketHand> range;
  for (unsigned i = upper_bound; i <= lower_bound; ++i) {
    range.insert(range.end(), buckets[i].begin(), buckets[i].end());
  }
  return range;
}

BucketCollection
BucketCollection::bucket_range(const unsigned &lower_bound,
                               const unsigned &upper_bound) const {
  BucketCollection range(0);
  for (int i = upper_bound; i <= lower_bound; ++i) {
    range.buckets.push_back(buckets[i]);
  }
  return range;
}

unsigned BucketCollection::nb_hands() const {
  int sum = 0;
  for (unsigned i = 0; i < buckets.size(); ++i)
    sum += buckets[i].size();
  return sum;
}

BucketCollection BucketCollection::remove_empty_buckets() const {
  vector<Bucket> newc;
  for (unsigned i = 0; i < buckets.size(); ++i)
    if (!buckets[i].empty())
      newc.push_back(buckets[i]);
  return BucketCollection(newc);
}

unsigned BucketCollection::nb_buckets() const { return buckets.size(); }

void BucketCollection::reverse() {
  std::reverse(buckets.begin(), buckets.end());
}

vector<BucketHand> BucketCollection::hands() const {
  return hand_bucket_range(0, nb_buckets() - 1);
}
}
