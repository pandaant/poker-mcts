#include "fseat.hpp"

namespace freedom {

FSeat::FSeat(const Value &data)
    :status(load_status(data)),
      player(FPlayer(data)) {}

StatusType::Enum FSeat::load_status(const Value &data) {
  string stat = data["status"].GetString();
  if (stat == "active")
    return StatusType::Active;
  if (stat == "inactive")
    return StatusType::Inactive;
  if (stat == "allin")
    return StatusType::Allin;
}

FSeat::FSeat(FPlayer _player, poker::StatusType::Enum _status)
    : status(_status), player(_player) {}

void FSeat::serialize(Writer<FileStream> &writer) {
  writer.StartArray();
  writer.String(StatusType::ToStrShort[status]);
  player.serialize(writer);
  writer.EndArray();
}

void FSeat::serialize_fields(Writer<FileStream> &writer) {
  writer.StartArray();
  writer.String("status");
  writer.String("name");
  writer.String("bankroll");
  writer.String("invested");
  writer.String("sequence");
  writer.EndArray();
}

bool FSeat::is_active() const { return status == StatusType::Active; }

bool FSeat::is_inactive() const { return status == StatusType::Inactive; }

bool FSeat::is_allin() const { return status == StatusType::Allin; }

void FSeat::set_inactive() { status = StatusType::Inactive; }

void FSeat::set_active() { status = StatusType::Active; }

void FSeat::set_allin() { status = StatusType::Allin; }
};
