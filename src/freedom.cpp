#include <iostream>
#include <string>
#include "freedom.hpp"
#include "weighted_bucket_handlist.hpp"
#include "ehs2_range_predictor.hpp"
#include "balanced_bucketizer.hpp"
#include "exponential_bucketizer.hpp"

namespace Freedom5 {

Freedom::Freedom(Handranks* handranks): handranks(handranks){
  ecalc = new ECalc(handranks);
  // fill with configurable vars TODO
  pr = new EHS2RangePredictor(
      ecalc, 1200,
      Threshold(1, 1), // TODO lie this to the other config options (was 2, 2)
      vector<int>({20, 10, 10, 10}), 10, false);
}

Freedom::~Freedom() {
  delete ecalc;
  delete pr;
}

void Freedom::generate_handranges(FContext &context, FConfig *config) {
  BalancedBucketizer bucketizer;
  for (int i = 0; i < context.seats.size(); ++i) {
    FPlayer *player = &context.seats[i].player;
    vector<unsigned> bothand{ context.config->bot_hand.highcard().card(), context.config->bot_hand.lowcard().card() };
    if (context.index_bot != i && !context.seats[i].is_inactive()) {
      // get hands to map them into buckets for the simulation
      vector<BucketHand> range = pr->_predict_range<ExponentialBucketizer>(
          player->action_sequence, config->models[player->model],
          context.config->board, bothand);

      BucketCollection bucket_range =
          bucketizer.map_hands(10, range); // TODO make this 10 and the 10 in
                                           // EHS2Predic.. from the same
                                           // variable
      // remove empty buckets
      bucket_range = bucket_range.remove_empty_buckets();
      vector<double> initial_weights(bucket_range.buckets.size());
      // weight is nb of hands in bucket. so each hand is sampled evenly 
      // at the start
      for(unsigned i = 0; i < bucket_range.buckets.size(); ++i)
          initial_weights[i] = bucket_range.buckets[i].size();

      //for(auto b : bucket_range.buckets){
        //for(auto h : b )
            //std::cout << h.to_str() << "\t" << h.equity << "\n";
        //std::cout << "\n";
      //}
      player->handlist = new WeightedBucketHandlist(
              // TODO change weights to nb hands in bucket
          bucket_range, initial_weights, player->action_sequence);
    }
  }
}

};
