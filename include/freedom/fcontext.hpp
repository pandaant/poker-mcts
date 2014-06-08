#ifndef FCONTEXT_H
#define FCONTEXT_H

#include <vector>
#include <decimal.h>
#include <poker/card.hpp>
#include <poker/action.hpp>
#include <poker/phase_type.hpp>
#include <poker/action_type.hpp>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/filestream.h>
#include "fplayer.hpp"
#include "fcontext_config.hpp"
#include "schema_exception.hpp"

namespace freedom {

using std::vector;
using rapidjson::Value;
using rapidjson::Writer;
using rapidjson::FileStream;
using rapidjson::SizeType;

// ----------------------------------------------------------------------
/// @brief   represents a pokersituation. the class can transition
///          for given actions and step through a complete game of poker.
// ----------------------------------------------------------------------
class FContext {
public:
  /// current pot in bigblinds
  bb pot;
  /// the highest bet that someone currently has bet
  bb highest_bet;
  /// index of bot player in the player vector
  int index_bot;
  /// first to act or last raiser
  int index_utg;
  int index_button;
  /// player that has to make a decision
  int index_active;
  unsigned betting_round;

  /// current phase we are in
  PhaseType::Enum phase;
  /// stores all players on the table.
  vector<FPlayer> player;
  /// the last action that led up to this context.
  Action last_action;
  FContextConfig *config;

  // ----------------------------------------------------------------------
  /// @brief   constructs a context from an json object
  ///
  /// @param data json
  /// @param _config the context config belonging to this context.
  // ----------------------------------------------------------------------
  FContext(const Value &data, FContextConfig *config_);

  FContext(const int &index_bot_, const int &index_utg_,
           const int &index_button_, const int &index_active_,
           const vector<FPlayer> &player_, FContextConfig *config_);

  FContext(const bb &pot_, const bb &highest_bet_, const int &index_bot_,
           const int &index_utg_, const int &index_button_,
           const int &index_active_, const int &betting_round_,
           const PhaseType::Enum &phase_, const vector<FPlayer> player_,
           const Action &last_action_, FContextConfig *config_);

  FContext(const FContext &fc);
  FContext &operator=(const FContext &fc);

  virtual ~FContext();

  // ----------------------------------------------------------------------
  /// @brief   if the round is over, the betting rounds are reset and the
  ///          next phase is set.
  // ----------------------------------------------------------------------
  void transition_phase();

  // ----------------------------------------------------------------------
  /// @brief   generates a new context, in which the currently active player
  ///          performed `action`.
  ///
  /// @param action action to perform
  ///
  /// @return new context in which player performed action.
  // ----------------------------------------------------------------------
  FContext transition(const Action &action) const;

  // ----------------------------------------------------------------------
  /// @brief   generates all possible contexts that can be derived from
  ///          the current context.
  ///
  /// @return all possible child contexts.
  // ----------------------------------------------------------------------
  virtual vector<FContext> transition() const;

  // ----------------------------------------------------------------------
  /// @brief   checks is a context is final state that cant be derived
  ///          any further.
  ///
  /// @return true if state is terminal.
  // ----------------------------------------------------------------------
  virtual bool is_terminal() const;

  // ----------------------------------------------------------------------
  /// @brief  get all actions that the currently active player can
  ///         perform.
  ///
  /// @return a list of enums describing actions.
  // ----------------------------------------------------------------------
  vector<ActionType::Enum> enum_available_actions() const;

  // ----------------------------------------------------------------------
  /// @brief   A list of all actions the currently active player
  ///          can perform. Includes duplicate actions in case of multiple
  ///          call or raise sizes.
  ///
  /// @return list of possible actions.
  // ----------------------------------------------------------------------
  vector<Action> available_actions() const;

  // ----------------------------------------------------------------------
  /// @brief   Checks if the current active player is last to act in the phase.
  ///
  /// @return false if some other player has to act after.
  // ----------------------------------------------------------------------
  bool is_last_to_act() const;

  // ----------------------------------------------------------------------
  /// @brief   checks if the current utg index can be reached by a transition.
  ///
  /// @return true if index is reachable
  // ----------------------------------------------------------------------
  bool is_utg_index_reachable() const;

  // ----------------------------------------------------------------------
  /// @brief   checks if the table is open ( someone has bet )
  ///
  /// @return true if someone has already bet
  // ----------------------------------------------------------------------
  bool has_bet() const;
  int nb_player_active() const;
  int nb_player_allin() const;
  int nb_player_inactive() const;
  int nb_player_not_inactive() const;

  // ----------------------------------------------------------------------
  /// @brief returns the index of the next player that has to act first.
  ///
  /// @return next utg index.
  // ----------------------------------------------------------------------
  int next_utg() const;

  // ----------------------------------------------------------------------
  /// @brief  index of player that is active after the current player
  ///         made his decision.
  ///
  ///
  /// @return next active index. -1 if no more to act
  // ----------------------------------------------------------------------
  int next_to_act() const;

  // ----------------------------------------------------------------------
  /// @brief   converts the context to a json document.
  ///
  /// @return context as json string
  // ----------------------------------------------------------------------
  string json() const;

  // ----------------------------------------------------------------------
  /// @brief   parses the phase from a string.
  ///
  /// @param data json
  ///
  /// @return phase enum
  // ----------------------------------------------------------------------
  PhaseType::Enum load_phase(const Value &data);

  // ----------------------------------------------------------------------
  /// @brief   parses players from a json string
  ///
  /// @param data json
  // ----------------------------------------------------------------------
  void load_players(const Value &data);

  // ----------------------------------------------------------------------
  /// @brief   multiplicates the passed factor with:
  ///           - highest_bet when someone has open raised
  ///           - pot when nobody has raised yet.
  ///
  /// @param factor to multiplicate with
  ///
  /// @return a bet or a raise amount.
  // ----------------------------------------------------------------------
  bb bet_raise_amount(const double &factor) const;

  // ----------------------------------------------------------------------
  /// @brief   gets ref to the player where the bot sits.
  ///
  /// @return bot player
  // ----------------------------------------------------------------------
  FPlayer &bot_player();
  const FPlayer &bot_player() const;

  // ----------------------------------------------------------------------
  /// @brief   gets ref to player where currently active player sits.
  ///
  /// @return current player
  // ----------------------------------------------------------------------
  FPlayer &active_player();
  const FPlayer &active_player() const;

  // ----------------------------------------------------------------------
  /// @brief   returns the first player that is still active.
  ///          this function should be only used if it is
  ///          known that exactly one player is still active.
  ///
  /// @return first player that is still active.
  // ----------------------------------------------------------------------
  const FPlayer &last_active_player() const;

  // ----------------------------------------------------------------------
  /// @brief   write all data fields to a stream
  ///
  /// @param writer object
  // ----------------------------------------------------------------------
  void serialize(Writer<FileStream> &writer) const;

  // ----------------------------------------------------------------------
  /// @brief   write the name of all data fields to a stream
  ///
  /// @param writer object
  // ----------------------------------------------------------------------
  void serialize_fields(Writer<FileStream> &writer);
};
};

#endif
