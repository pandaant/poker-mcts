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

  FPlayer(const bb &bankroll_);

  FPlayer(const bb &bankroll_, const StatusType::Enum &status_);

  FPlayer(const bb &bankroll_, const vector<bb> &invested_);

  FPlayer(const bb &bankroll_, const vector<bb> &invested_,
          StatusType::Enum status_);

  FPlayer(const bb &bankroll_, const vector<bb> &invested_, Handlist *handlist_,
          const string &model_);

  FPlayer(const bb &bankroll_, const vector<bb> &invested_, Handlist *handlist_,
          const string &model_, const StatusType::Enum &status_);

  FPlayer(const FPlayer &p);

  FPlayer &operator=(const FPlayer &p);

  ~FPlayer();

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
