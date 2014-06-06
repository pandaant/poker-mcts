#ifndef FACTION_SEQUENCE_H
#define FACTION_SEQUENCE_H

#include <rapidjson/document.h>
#include <poker/action_sequence.hpp>

namespace freedom {

using namespace poker;
using rapidjson::Value;
using rapidjson::SizeType;

// ----------------------------------------------------------------------
/// @brief   extends the action sequence with the ability to get init.
///          from a json object.
// ----------------------------------------------------------------------
class FActionSequence : public ActionSequence {
public:
  explicit FActionSequence(const Value &data);

  // ----------------------------------------------------------------------
  /// @brief   converts an action as string to an enum type.
  ///
  /// @param action action to lookup
  ///
  /// @return action as enum
  // ----------------------------------------------------------------------
  ActionType::Enum resolve_action(const string &action) const;
};
}

#endif
