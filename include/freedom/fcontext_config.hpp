/*
 * File:   fcontext_config.h
 * Author: batman
 *
 * Created on August 8, 2013, 6:45 PM
 */

#ifndef FCONTEXT_CONFIG_H
#define FCONTEXT_CONFIG_H

#include <vector>
#include <poker/pokerdefs.hpp>
#include <poker/hand.hpp>
#include <schema_exception.hpp>
#include <rapidjson/document.h>

namespace Freedom5 {
using std::vector;
using rapidjson::Value;
using rapidjson::SizeType;

using namespace poker;

/**
 * contains data that is used in every
 * context in the tree.
 **/
struct FContextConfig {
  int betting_rounds;
  double rake_factor;
  Hand bot_hand;
  vector<unsigned> board;
  vector<double> bet_sizes;
  vector<double> raise_sizes;

  FContextConfig(const Value &data);

  FContextConfig(Hand _bot_hand, int _betting_rounds, vector<unsigned> _board,
                 vector<double> _bet_sizes, vector<double> _raise_sizes,
                 double _rake_factor = 0)
      : bot_hand(_bot_hand), betting_rounds(_betting_rounds),
        rake_factor(_rake_factor), board(_board), bet_sizes(_bet_sizes),
        raise_sizes(_raise_sizes) {}

  FContextConfig(const FContextConfig &oc)
      : bot_hand(oc.bot_hand), betting_rounds(oc.betting_rounds),
        board(oc.board), bet_sizes(oc.bet_sizes), raise_sizes(oc.raise_sizes) {}

  ~FContextConfig();
};
};

#endif /* FCONTEXT_CONFIG_H */

