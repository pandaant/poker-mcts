#ifndef FPLAYER_H
#define FPLAYER_H

#include <string>
#include <vector>
#include <decimal.h>
#include <poker/status_type.hpp>
#include <ecalc/handlist.hpp>
#include "faction_sequence.hpp"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filestream.h>

namespace freedom {

using ecalc::Handlist;
using rapidjson::Value;
using rapidjson::Writer;
using rapidjson::FileStream;
using std::string;
using std::vector;

using namespace poker;

class FPlayer {
public:
  string name;
  bb bankroll;
  vector<bb> invested;
  string model;
  ActionSequence action_sequence;
  Handlist *handlist;
  StatusType::Enum status;

  FPlayer(const Value &data);

  ~FPlayer() {}

  FPlayer(string _name, bb _bankroll)
      : FPlayer(_name, _bankroll, vector<bb>(4, bb(0)), NULL, "default",
                StatusType::Active) {}

  FPlayer(string _name, bb _bankroll, vector<bb> _invested)
      : FPlayer(_name, _bankroll, _invested, NULL, "default",
                StatusType::Active) {}

  FPlayer(string _name, bb _bankroll, vector<bb> invested_, Handlist *_handlist,
          string _model)
      : FPlayer(_name, _bankroll, invested_, _handlist, _model,
                StatusType::Active) {}

  FPlayer(string _name, bb _bankroll, vector<bb> invested_,
          StatusType::Enum status_)
      : FPlayer(_name, _bankroll, invested_, NULL, "default", status_) {}

  FPlayer(string _name, bb _bankroll, StatusType::Enum status_)
      : FPlayer(_name, _bankroll, vector<bb>(4, bb(0)), NULL, "default",
                status_) {}

  FPlayer(string _name, bb _bankroll, vector<bb> invested_, Handlist *_handlist,
          string _model, StatusType::Enum status_)
      : invested(invested_), name(_name), bankroll(_bankroll),
        handlist(_handlist), model(_model), status(status_) {}

  FPlayer(const FPlayer &p)
      : bankroll(p.bankroll), name(p.name), invested(p.invested),
        action_sequence(p.action_sequence), handlist(p.handlist),
        model(p.model), status(p.status) {}

  FPlayer &operator=(const FPlayer &p);

  bool make_investment(const bb &amount, const PhaseType::Enum &phase);
  bb total_investment() const;

  bool is_active() const;
  bool is_inactive() const;
  bool is_allin() const;

  void set_inactive();
  void set_active();
  void set_allin();

  StatusType::Enum load_status(const Value &data) const;
  void serialize(Writer<FileStream> &writer) const;
  static void serialize_fields(Writer<FileStream> &writer);
};
}

#endif
