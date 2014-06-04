#include "weighted_bucket_handlist.hpp"
#include <iostream>

namespace freedom {
using namespace poker;
ecalc::combination WeightedBucketHandlist::get_hand(
    ecalc::XOrShiftGenerator &nb_gen, ecalc::bitset &deck) {
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
}
}
