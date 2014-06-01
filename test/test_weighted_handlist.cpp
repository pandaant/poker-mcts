#include <map>
#include <iostream>
#include <UnitTest++.h>
#include <weighted_bucket_handlist.hpp>

SUITE(WeightedBucketHandlistTest) {
  using namespace Freedom5;
  using namespace ecalc;

  struct Setup {
    ecalc::XOrShiftGenerator nb_gen;
    ecalc::bitset deck_mask;

    Setup() {
      // seeded with zero, so results are independent of randomness
      nb_gen = ecalc::XOrShiftGenerator();
      deck_mask = ecalc::bitset();
      deck_mask = DECK_M;
    }
  };

  TEST_FIXTURE(Setup,SimpleCreate) {
    //Poker::Handlist::GeneratorType *gen = new Poker::Handlist::GeneratorType(0);    
    BucketCollection buckets(2);
    Hand hand_a("AhAs");
    Hand hand_b("KhKs");
    buckets.buckets[0].push_back(BucketHand(hand_a,1));
    buckets.buckets[1].push_back(BucketHand(hand_b,1));
    WeightedBucketHandlist handlist(buckets, vector<double>({1, 1}));

    // get 1000 hands from handlist
    unsigned nb_hands = 1000;
    std::map<ecalc::combination, int> hands;
    ecalc::bitset deck_mask_cpy;
    for (unsigned i = 0; i < nb_hands; ++i) {
      deck_mask_cpy = deck_mask; // make copy, so we dont run out of cards.
      ++hands[handlist.get_hand(nb_gen, deck_mask_cpy)];
    }

    // should be 50:50
    CHECK(((++hands.begin()) != hands.end()));
    double ratio = hands.begin()->second / ((++hands.begin())->second + 0.0);
    CHECK_CLOSE(1,ratio,0.1);
  }

  TEST_FIXTURE(Setup,SimpleCreate2) {
    //Poker::Handlist::GeneratorType *gen = new Poker::Handlist::GeneratorType(0);    
    BucketCollection buckets(2);
    Hand hand_a("AhAs");
    Hand hand_b("KhKs");
    buckets.buckets[0].push_back(BucketHand(hand_a,1));
    buckets.buckets[1].push_back(BucketHand(hand_b,1));
    // first bucket 2 times more than second
    WeightedBucketHandlist handlist(buckets, vector<double>({2, 1})); 

    // get 1000 hands from handlist
    unsigned nb_hands = 1000;
    std::map<ecalc::combination, int> hands;
    ecalc::bitset deck_mask_cpy;
    for (unsigned i = 0; i < nb_hands; ++i) {
      deck_mask_cpy = deck_mask; // make copy, so we dont run out of cards.
      ++hands[handlist.get_hand(nb_gen, deck_mask_cpy)];
    }

    // should be 50:50
    CHECK(((++hands.begin()) != hands.end()));
    double ratio =
        hands[CREATE_HAND(hand_a.highcard().card(), hand_a.lowcard().card())] /
        (hands[CREATE_HAND(hand_b.highcard().card(), hand_b.lowcard().card())] + 0.);
    CHECK_CLOSE(2,ratio,0.1);
  }
}

