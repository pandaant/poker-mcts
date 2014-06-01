/*
 * File:   fplayer.h
 * Author: batman
 *
 * Created on August 8, 2013, 2:31 PM
 */

#ifndef FPLAYER_H
#define FPLAYER_H

#include <string>
#include <vector>
#include <decimal.h>
#include <ecalc/handlist.hpp>
#include "faction_sequence.hpp"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filestream.h>

namespace Freedom5 {

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

  FPlayer(const Value &data);

  ~FPlayer(){}

  FPlayer(string _name)
      : invested(4, bb(0)), name(_name), handlist(NULL), model("default") {}

  FPlayer(string _name, bb _bankroll)
      : name(_name), bankroll(_bankroll), invested(4, bb(0)), handlist(NULL),
        model("default") {}

  FPlayer(string _name, bb _bankroll, Handlist *_handlist)
      : name(_name), bankroll(_bankroll), invested(4, bb(0)),
        handlist(_handlist), model("default") {}

  FPlayer(string _name, bb _bankroll, vector<bb> _invested)
      : name(_name), bankroll(_bankroll), invested(_invested), handlist(NULL),
        model("default") {}

  FPlayer(string _name, bb _bankroll, vector<bb> _invested,
          Handlist *_handlist)
      : invested(4, bb(0)), name(_name), bankroll(_bankroll),
        handlist(_handlist), model("default") {}

  FPlayer(string _name, bb _bankroll, vector<bb> _invested,
          Handlist *_handlist, string _model)
      : invested(4, bb(0)), name(_name), bankroll(_bankroll),
        handlist(_handlist), model(_model) {}

  FPlayer(const FPlayer &p)
      : bankroll(p.bankroll), name(p.name), invested(p.invested),
        action_sequence(p.action_sequence), handlist(p.handlist),
        model(p.model) {}

  FPlayer &operator=(const FPlayer &p);

  bool make_investment(bb amount, PhaseType::Enum phase);
  bb total_investment() const;


  void serialize(Writer<FileStream> &writer);
};
};

#endif /* FPLAYER_H */

