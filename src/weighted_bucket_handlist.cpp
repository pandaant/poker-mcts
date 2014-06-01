#include "weighted_bucket_handlist.hpp"
#include <iostream>

namespace Freedom5 {
using namespace poker;
ecalc::combination WeightedBucketHandlist::get_hand(
    ecalc::XOrShiftGenerator &nb_gen, ecalc::bitset &deck) {
  unsigned counter = MAX_TRIES;
  while (counter > 0) {
    try {
      int bucket = dist(nb_gen);
      return buckets[bucket].get_hand(nb_gen, deck);
    }
    catch (std::logic_error &e) {
      --counter;
    }
  }
}
//void WeightedBucketHandlist::get_hands(HandCollection &hands) {
  //for (unsigned i = 0; i < buckets.size(); ++i)
    //buckets[i].get_hands(hands);
//}
}
