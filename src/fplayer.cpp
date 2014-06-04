#include "fplayer.hpp"
#include <ecalc/random_handlist.hpp>

namespace freedom {

FPlayer::FPlayer(const Value &data) {
  name = data["name"].GetString();
  model = data["model"].GetString();
  bankroll = bb(data["bankroll"].GetDouble());
  action_sequence = FActionSequence(data["sequence"]);

  for (SizeType i = 0; i < data["invested"].Size(); ++i) {
    bb amount = bb(data["invested"][i].GetDouble());
    invested[i] = amount;
  }

  handlist = NULL;
}

FPlayer &FPlayer::operator=(const FPlayer &p) {
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
  writer.String(name.c_str());
  writer.Double(bankroll.getAsDouble());

  writer.StartArray();
  for (auto &i : invested)
    writer.Double(i.getAsDouble());
  writer.EndArray();

  writer.String(action_sequence.str().c_str());
}
};
