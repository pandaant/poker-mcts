/*
 * File:   faction_sequence.h
 * Author: batman
 *
 * Created on August 18, 2013, 4:46 PM
 */

#ifndef FACTION_SEQUENCE_H
#define FACTION_SEQUENCE_H

#include <poker/action_sequence.hpp>
#include <rapidjson/document.h>

namespace freedom {

using namespace poker;
using rapidjson::Value;
using rapidjson::SizeType;

class FActionSequence : public ActionSequence {
public:
  FActionSequence(const Value &data);
  ActionType::Enum resolve_action(string action);
};
};

#endif /* FACTION_SEQUENCE_H */

