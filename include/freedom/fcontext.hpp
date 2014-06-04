/*
 * File:   fcontext.h
 * Author: batman
 *
 * Created on August 7, 2013, 11:52 PM
 */

#ifndef FCONTEXT_H
#define FCONTEXT_H

#include <schema_exception.hpp>
#include <vector>
#include <decimal.h>
#include <poker/card.hpp>
#include <poker/action.hpp>
#include <poker/phase_type.hpp>
#include <poker/action_type.hpp>
#include "fcontext_config.hpp"
#include "double_operations.hpp"
#include "fseat.hpp"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filestream.h>

namespace freedom {

using rapidjson::Value;
using rapidjson::Writer;
using rapidjson::FileStream;
using rapidjson::SizeType;
using std::vector;
using namespace poker;

class FContext{
public:
  bb pot;
  bb highest_bet;
  int index_bot;
  // first to act or last raiser
  int index_utg;
  int index_button;
  int index_active;
  int betting_round;

  PhaseType::Enum phase;
  vector<FSeat> seats;

  Action last_action;
  FContextConfig *config;

  FContext(const Value &data, FContextConfig *_config);

  FContext(int _index_bot, int _index_utg, int _index_button, int _index_active,
           vector<FSeat> _seats, FContextConfig *_config);
  FContext(bb _pot, bb _highest_bet, int _index_bot, int _index_utg,
           int _index_button, int _index_active, int _betting_round,
           PhaseType::Enum _phase, vector<FSeat> _seats, Action _last_action,
           FContextConfig *_config);
  FContext(const FContext &fc);
  FContext& operator=(const FContext &fc);
  virtual ~FContext();

  void transition_phase();
  
  /**
   * generates a new context, in which the currently active player
   * performed `action`. 
   * @param action
   * @return 
   */
  FContext transition(Action action) const;
  
  /**
   * generates all possible contexts that can be derived from
   * the current context.
   * @return list of all possible contexts derivable 
   */
  virtual vector<FContext> transition() const;
  
  /**
   * checks if context is a final gamestate.
   * @return true if final
   */
  virtual bool is_terminal() const;

  /**
   * Vector of action enums of all actions the 
   * currently active player can perform
   */
  vector<ActionType::Enum> enum_available_actions() const;

  /**
   * a vector of all actions the currently active player
   * can perform.
   * Includes duplicate actions in case of multiple
   * call or raise sizes
   * @return 
   */
  vector<Action> available_actions() const;

  /**
   * checks if the current active player is last to act in the phase.
   * returns false if some other player has to act after.
   * @return
   */
  bool is_last_to_act() const;
  bool is_utg_index_reachable() const;
  bool has_bet() const;
  int nb_player_active() const;
  int nb_player_allin() const;
  int nb_player_inactive() const;
  int nb_player_not_inactive() const;
  int next_utg() const;
  int next_to_act() const;
  string json();

  PhaseType::Enum load_phase(const Value &data);
  void load_players(const Value &data);

  /**
   * multiplicates the passed factor with:
   *  - highest_bet when someone has open raised
   *  - pot when nobody has raised yet.
   * @param factor
   * @return
   */
  bb get_bet_raise_amount(double factor) const;

  FSeat &bot_seat();
  const FSeat &bot_seat() const;
  FSeat &active_seat();
  const FSeat &active_seat() const;

  /**
   * returns the first seat that is still active.
   * this function should be only used if it is
   * known that exactly one seat is still active.
   * 
   * IMPORTANT: this function returns a copy of
   * the seat. be careful when using this. 
   */
  FSeat get_last_active_seat() const;

  virtual FContext clone() const { return FContext(*this); }

  void serialize(Writer<FileStream> &writer);
  void serialize_fields(Writer<FileStream> &writer);
};
};

#endif /* FCONTEXT_H */

