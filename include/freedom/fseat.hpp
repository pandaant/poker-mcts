/*
 * File:   fseat.h
 * Author: batman
 *
 * Created on August 8, 2013, 3:05 PM
 */

#ifndef FSEAT_H
#define FSEAT_H

#include <poker/status_type.hpp>
#include "fplayer.hpp"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filestream.h>

/**
 die klasse wieder rausnehmen, die bring nur unnötige indirektionen rein
 * kann alles in die fplayer klasse TODO
 */
namespace Freedom5 {

using rapidjson::Value;
using rapidjson::Writer;
using rapidjson::FileStream;
using namespace poker;

class FSeat {
public:
  FPlayer player;
  StatusType::Enum status;

  FSeat(const Value &data);

  FSeat(FPlayer _player, poker::StatusType::Enum _status);

  FSeat(const FSeat &s) : status(s.status), player(s.player) {}
  FSeat operator=(const FSeat &s) {
    status = s.status;
    player = s.player;
    return *this;
  }

  ~FSeat() {}

  StatusType::Enum load_status(const Value &data);
  void serialize(Writer<FileStream> &writer);
  static void serialize_fields(Writer<FileStream> &writer);

  bool is_active() const;
  bool is_inactive() const;
  bool is_allin() const;

  void set_inactive();
  void set_active();
  void set_allin();
};
};

#endif /* FSEAT_H */

