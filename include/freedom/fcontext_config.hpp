#ifndef FCONTEXT_CONFIG_H
#define FCONTEXT_CONFIG_H

#include <vector>
#include <poker/hand.hpp>
#include <poker/pokerdefs.hpp>
#include <ecalc/types.hpp>
#include <rapidjson/document.h>
#include "schema_exception.hpp"

namespace freedom {
using std::vector;
using ecalc::card;
using ecalc::cards;
using poker::Hand;
using rapidjson::Value;
using rapidjson::SizeType;

// ----------------------------------------------------------------------
/// @brief   Contains context data that is fixed for the entire duration
///          of a game.
// ----------------------------------------------------------------------
struct FContextConfig {
  /// number of max bettingrounds a phase can have.
  unsigned betting_rounds;
  /// rake factor is removed from a simulated pot. (1-rake_f)*pot = real pot
  double rake_factor;
  /// the hand the bot holds in the game
  Hand bot_hand;
  /// community cards
  cards board;
  /// each entry is created as bet node.
  vector<double> bet_sizes;
  /// each entry is created as raise node.
  vector<double> raise_sizes;

  explicit FContextConfig(const Value &data);

  FContextConfig(const Hand &bot_hand_, const unsigned &betting_rounds_,
                 const cards &board_, const vector<double> &bet_sizes_,
                 const vector<double> &raise_sizes_,
                 const double &rake_factor_ = 0);

  FContextConfig(const FContextConfig &oc);

  ~FContextConfig();
};
}

#endif
