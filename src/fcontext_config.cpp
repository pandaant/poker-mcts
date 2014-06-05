#include "fcontext_config.hpp"

namespace freedom {

FContextConfig::FContextConfig(Hand _bot_hand, unsigned _betting_rounds,
                               vector<card> _board, vector<double> _bet_sizes,
                               vector<double> _raise_sizes, double _rake_factor)
    : betting_rounds(_betting_rounds), rake_factor(_rake_factor),
      bot_hand(_bot_hand), board(_board), bet_sizes(_bet_sizes),
      raise_sizes(_raise_sizes) {}

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
