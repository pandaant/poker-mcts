#ifndef FCONTEXT_CONFIG_H
#define FCONTEXT_CONFIG_H

#include <vector>
#include <poker/hand.hpp>
#include <poker/pokerdefs.hpp>
#include <ecalc/types.hpp>
#include <schema_exception.hpp>
#include <rapidjson/document.h>

namespace freedom {
using std::vector;
using ecalc::card;
using poker::Hand;
using rapidjson::Value;
using rapidjson::SizeType;

/**
 * contains data that is used in every
 * context in the tree.
 **/
struct FContextConfig {
  unsigned betting_rounds;
  double rake_factor;
  Hand bot_hand;
  vector<card> board;
  vector<double> bet_sizes;
  vector<double> raise_sizes;

  explicit FContextConfig(const Value &data);

  FContextConfig(Hand _bot_hand, unsigned _betting_rounds, vector<unsigned> _board,
                 vector<double> _bet_sizes, vector<double> _raise_sizes,
                 double _rake_factor = 0);

  FContextConfig(const FContextConfig &oc);

  ~FContextConfig();
};
}

#endif
