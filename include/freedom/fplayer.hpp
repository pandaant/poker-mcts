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
  bb bankroll;
  vector<bb> invested;
  ActionSequence action_sequence;
  Handlist *handlist;
  string model;
  StatusType::Enum status;

  FPlayer(const Value &data);

  ~FPlayer() {}

  FPlayer(bb _bankroll)
      : FPlayer(_bankroll, vector<bb>(4, bb(0)), NULL, "default",
                StatusType::Active) {}

  FPlayer(bb _bankroll, vector<bb> _invested)
      : FPlayer(_bankroll, _invested, NULL, "default", StatusType::Active) {}

  FPlayer(bb _bankroll, vector<bb> invested_, Handlist *_handlist,
          string _model)
      : FPlayer(_bankroll, invested_, _handlist, _model, StatusType::Active) {}

  FPlayer(bb _bankroll, vector<bb> invested_, StatusType::Enum status_)
      : FPlayer(_bankroll, invested_, NULL, "default", status_) {}

  FPlayer(bb _bankroll, StatusType::Enum status_)
      : FPlayer(_bankroll, vector<bb>(4, bb(0)), NULL, "default", status_) {}

  FPlayer(bb _bankroll, vector<bb> invested_, Handlist *_handlist,
          string _model, StatusType::Enum status_)
      : bankroll(_bankroll), invested(invested_), handlist(_handlist),
        model(_model), status(status_) {}

  FPlayer(const FPlayer &p)
      : bankroll(p.bankroll), invested(p.invested),
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
