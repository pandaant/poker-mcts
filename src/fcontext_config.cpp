#include "fcontext_config.hpp"

namespace Freedom5 {

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
