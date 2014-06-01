#include "fplayer.hpp"
#include <ecalc/random_handlist.hpp>

namespace Freedom5 {

FPlayer::FPlayer(const Value &data) {
  name = data["name"].GetString();
  this->model = data["model"].GetString();
  bankroll = bb(data["bankroll"].GetDouble());
  action_sequence = FActionSequence(data["sequence"]);

  for (SizeType i = 0; i < data["invested"].Size(); ++i) {
    bb amount = bb(data["invested"][i].GetDouble());
    invested[i] = amount;
  }
  //Player::Player(name, bankroll, invested);

  handlist = NULL;
}

FPlayer &FPlayer::operator=(const FPlayer &p) {
  //Player::operator=(p);
  name = p.name;
  bankroll = p.bankroll;
  invested = p.invested;
  action_sequence = p.action_sequence;
  handlist = p.handlist;
  model = p.model;
  return *this;
}

bool FPlayer::make_investment(bb amount, PhaseType::Enum phase) {
  if (bankroll < amount)
    return false;
  bankroll -= amount;
  invested[(int)phase] += amount;
  return true;
}

bb FPlayer::total_investment() const {
  bb sum(0);
  for (int i = 0; i < 4; ++i)
    sum += invested[i];
  return sum;
}

void FPlayer::serialize(Writer<FileStream> &writer) {
  // writer.String("name");
  writer.String(name.c_str());

  // writer.String("bankroll");
  writer.Double(bankroll.getAsDouble());

  // writer.String("invested");
  writer.StartArray();
  for (auto &i : invested)
    writer.Double(i.getAsDouble());
  writer.EndArray();

  // writer.String("sequence");
  writer.String(action_sequence.str().c_str());
}
};
