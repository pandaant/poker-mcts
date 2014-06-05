#include <iostream>
#include <string>
#include "freedom.hpp"
#include "weighted_bucket_handlist.hpp"
#include "ehs2_range_predictor.hpp"
#include "balanced_bucketizer.hpp"
#include "exponential_bucketizer.hpp"

namespace freedom {

Freedom::Freedom(Handranks *handranks_)
    : handranks(handranks_), ecalc(new ECalc(handranks_)) {
  // fill with configurable vars TODO
  range_predictor = new EHS2RangePredictor(
      ecalc, 1200,
      Threshold(1, 1), // TODO lie this to the other config options (was 2, 2)
      vector<int>({20, 10, 10, 10}), 10, false);
}

Freedom::~Freedom() {
  delete ecalc;
  delete range_predictor;
}

void Freedom::generate_handranges(FContext &context, FConfig *config) {
  BalancedBucketizer bucketizer;
  for (int i = 0; i < context.player.size(); ++i) {
    FPlayer *player = &context.player[i];
    vector<unsigned> bothand{context.config->bot_hand.highcard().card(),
                             context.config->bot_hand.lowcard().card()};
    if (context.index_bot != i && !context.player[i].is_inactive()) {
      // get hands to map them into buckets for the simulation
      vector<BucketHand> range = range_predictor->_predict_range<ExponentialBucketizer>(
          player->action_sequence, config->models[player->model],
          context.config->board, bothand);

      BucketCollection bucket_range =
          bucketizer.map_hands(10, range); // TODO make this 10 and the 10 in
                                           // EHS2Predic.. from the same
                                           // variable
      // remove empty buckets
      bucket_range = bucket_range.remove_empty_buckets();
      vector<double> initial_weights(bucket_range.nb_buckets());
      // weight is nb of hands in bucket. so each hand is sampled evenly
      // at the start
      for (unsigned i = 0; i < bucket_range.nb_buckets(); ++i)
        initial_weights[i] = bucket_range[i].size();

      player->handlist = new WeightedBucketHandlist(
          // TODO change weights to nb hands in bucket
          bucket_range, initial_weights, player->action_sequence);
    }
  }
}
}
