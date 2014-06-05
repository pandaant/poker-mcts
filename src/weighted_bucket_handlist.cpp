#include "weighted_bucket_handlist.hpp"

namespace freedom {
using namespace poker;

WeightedBucketHandlist::WeightedBucketHandlist(
    const BucketCollection &buckets_, const vector<double> &weights_,
    const ActionSequence &range_sequence_)
    : weights(weights_), dist(weights_.begin(), weights_.end()),
      range_sequence(range_sequence_), buckets(convert_buckets(buckets_)) {}

WeightedBucketHandlist::WeightedBucketHandlist(const BucketCollection &buckets_,
                                               const vector<double> &weights_)
    : weights(weights_), dist(weights_.begin(), weights_.end()),
      range_sequence(), buckets(convert_buckets(buckets_)) {}

WeightedBucketHandlist::WeightedBucketHandlist(
    const WeightedBucketHandlist &wbh, const vector<double> &new_weights)
    : weights(new_weights), dist(new_weights.begin(), new_weights.end()),
      range_sequence(wbh.range_sequence), buckets(wbh.buckets) {}

combination WeightedBucketHandlist::get_hand(ecalc::XOrShiftGenerator &nb_gen,
                                             ecalc::bitset &deck) {
  unsigned counter = MAX_TRIES;
  while (counter > 0) {
    try {
      int bucket = dist(nb_gen);
      return buckets[bucket].get_hand(nb_gen, deck);
    }
    catch (std::runtime_error &e) {
      --counter;
    }
  }
  throw std::runtime_error("No Hand assignable");
}

vector<ecalc::ArrayHandlist> WeightedBucketHandlist::convert_buckets(
    const BucketCollection &collection) const {
  vector<ecalc::ArrayHandlist> handlists;
  for (unsigned i = 0; i < collection.nb_buckets(); ++i) {
    vector<poker::Hand> range;
    for (unsigned j = 0; j < collection[i].size(); ++j) {
      range.push_back(static_cast<Hand>(collection[i][j]));
    }
    handlists.push_back(ecalc::ArrayHandlist(range));
  }
  return handlists;
}
}
