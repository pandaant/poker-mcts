#include "fcontext_config.hpp"

namespace freedom {

FContextConfig::FContextConfig(const Hand &bot_hand_,
                               const unsigned &betting_rounds_,
                               const vector<card> &board_,
                               const vector<double> &bet_sizes_,
                               const vector<double> &raise_sizes_,
                               const double &rake_factor_)
    : betting_rounds(betting_rounds_), rake_factor(rake_factor_),
      bot_hand(bot_hand_), board(board_), bet_sizes(bet_sizes_),
      raise_sizes(raise_sizes_) {}

FContextConfig::FContextConfig(const FContextConfig &oc)
    : betting_rounds(oc.betting_rounds), bot_hand(oc.bot_hand), board(oc.board),
      bet_sizes(oc.bet_sizes), raise_sizes(oc.raise_sizes) {}

FContextConfig::FContextConfig(const Value &data)
    : bot_hand(
          Hand(data["bot_hand"][0u].GetInt(), data["bot_hand"][1u].GetInt())) {
  betting_rounds = data["betting_rounds"].GetInt();
  rake_factor = data["rake"].GetDouble();

  for (SizeType i = 0; i < data["board"].Size(); i++)
    board.push_back(data["board"][i].GetInt());

  for (SizeType i = 0; i < data["bet_sizes"].Size(); i++)
    bet_sizes.push_back(data["bet_sizes"][i].GetDouble());

  for (SizeType i = 0; i < data["raise_sizes"].Size(); i++)
    raise_sizes.push_back(data["raise_sizes"][i].GetDouble());
}

FContextConfig::~FContextConfig() {}
};
