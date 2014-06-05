#ifndef FACTION_SEQUENCE_H
#define FACTION_SEQUENCE_H

#include <rapidjson/document.h>
#include <poker/action_sequence.hpp>

namespace freedom {

using namespace poker;
using rapidjson::Value;
using rapidjson::SizeType;

class FActionSequence : public ActionSequence {
public:
  FActionSequence(const Value &data);
  ActionType::Enum resolve_action(const string &action) const;
};
}

#endif
