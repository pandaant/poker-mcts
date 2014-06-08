#include "fplayer.hpp"
#include <ecalc/random_handlist.hpp>

namespace freedom {

FPlayer::FPlayer(const Value &data) : status(load_status(data)){
  model = data["model"].GetString();
  bankroll = bb(data["bankroll"].GetDouble());
  action_sequence = FActionSequence(data["sequence"]);

  for (SizeType i = 0; i < data["invested"].Size(); ++i) {
    bb amount = bb(data["invested"][i].GetDouble());
    invested.push_back(amount);
  }

  handlist = NULL;
}

FPlayer::FPlayer(const bb &bankroll_, const vector<bb> &invested_,
                 Handlist *handlist_, const string &model_,
                 const StatusType::Enum &status_)
    : bankroll(bankroll_), invested(invested_), handlist(handlist_),
      model(model_), status(status_) {}

FPlayer::FPlayer(const bb &bankroll_)
    : FPlayer(bankroll_, vector<bb>(4, bb(0)), NULL, "default",
              StatusType::Active) {}

FPlayer::FPlayer(const bb &bankroll_, const vector<bb> &invested_)
    : FPlayer(bankroll_, invested_, NULL, "default", StatusType::Active) {}

FPlayer::FPlayer(const bb &bankroll_, const vector<bb> &invested_,
                 Handlist *handlist_, const string &model_)
    : FPlayer(bankroll_, invested_, handlist_, model_, StatusType::Active) {}

FPlayer::FPlayer(const bb &bankroll_, const vector<bb> &invested_,
                 StatusType::Enum status_)
    : FPlayer(bankroll_, invested_, NULL, "default", status_) {}

FPlayer::FPlayer(const bb &bankroll_, const StatusType::Enum &status_)
    : FPlayer(bankroll_, vector<bb>(4, bb(0)), NULL, "default", status_) {}

FPlayer::FPlayer(const FPlayer &p)
    : bankroll(p.bankroll), invested(p.invested),
      action_sequence(p.action_sequence), handlist(p.handlist), model(p.model),
      status(p.status) {}

FPlayer &FPlayer::operator=(const FPlayer &p) {
  bankroll = p.bankroll;
  invested = p.invested;
  action_sequence = p.action_sequence;
  handlist = p.handlist;
  model = p.model;
  status = p.status;
  return *this;
}

FPlayer::~FPlayer() {}

bool FPlayer::make_investment(const bb &amount, const PhaseType::Enum &phase) {
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

bool FPlayer::is_active() const { return status == StatusType::Active; }

bool FPlayer::is_inactive() const { return status == StatusType::Inactive; }

bool FPlayer::is_allin() const { return status == StatusType::Allin; }

void FPlayer::set_inactive() { status = StatusType::Inactive; }

void FPlayer::set_active() { status = StatusType::Active; }

void FPlayer::set_allin() { status = StatusType::Allin; }

StatusType::Enum FPlayer::load_status(const Value &data) const {
  string stat = data["status"].GetString();
  if (stat == "active")
    return StatusType::Active;
  if (stat == "inactive")
    return StatusType::Inactive;
  if (stat == "allin")
    return StatusType::Allin;
  throw std::runtime_error("Statustype unknown");
}

void FPlayer::serialize_fields(Writer<FileStream> &writer) {
  writer.StartArray();
  writer.String("status");
  writer.String("bankroll");
  writer.String("invested");
  writer.String("sequence");
  writer.EndArray();
}

void FPlayer::serialize(Writer<FileStream> &writer) const {
  writer.String(StatusType::ToStrShort[status]);
  writer.Double(bankroll.getAsDouble());

  writer.StartArray();
  for (auto &i : invested)
    writer.Double(i.getAsDouble());
  writer.EndArray();

  writer.String(action_sequence.str().c_str());
}
};
