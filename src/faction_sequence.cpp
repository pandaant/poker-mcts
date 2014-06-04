#include "faction_sequence.hpp"

namespace freedom {

FActionSequence::FActionSequence(const Value &data) : ActionSequence() {
  for (SizeType i = 0; i < 4; i++) {
    for (SizeType j = 0; j < data[i].Size(); j++) {
      PhaseType::Enum phase = static_cast<PhaseType::Enum>(i);
      Action action(resolve_action(data[i][j][0u].GetString()),
                    bb(data[i][j][1u].GetDouble()));
      append(action, phase, data[i][j][2u].GetInt());
    }
  }
}

ActionType::Enum FActionSequence::resolve_action(string action) {
  if (action == "X")
    return ActionType::Check;
  if (action == "F")
    return ActionType::Fold;
  if (action == "C")
    return ActionType::Call;
  if (action == "R")
    return ActionType::Raise;
  if (action == "B")
    return ActionType::Bet;
  return ActionType::None;
}
};
