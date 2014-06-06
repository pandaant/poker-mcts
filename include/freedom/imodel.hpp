#ifndef MODEL_H
#define MODEL_H

#include <tuple>
#include <string>
#include "fcontext.hpp"

namespace freedom {

using std::tuple;
using std::string;

class IModel {
public:
  virtual ~IModel() {}

  // ----------------------------------------------------------------------
  /// @brief   gets the fold/call and raise probabilites for a given
  ///          context and player according to the model.
  ///
  /// @param gs a context
  /// @param index_player the player to get probabilities for
  ///
  /// @return probability distribution tuple
  // ----------------------------------------------------------------------
  virtual tuple<double, double, double>
  getFoldCallRaiseProbabilities(const FContext &gs) const = 0;

  // ----------------------------------------------------------------------
  /// @brief   gets the check/bet probabilities for a context.
  ///
  /// @param gs a context 
  /// @param index_player the player to get probabilities for
  ///
  /// @return probability distribution tuple
  // ----------------------------------------------------------------------
  virtual tuple<double, double>
  getCheckBetProbabilities(const FContext &gs) const = 0;

  // ----------------------------------------------------------------------
  /// @brief   gets the fold/call probabilities for a context.
  ///
  /// @param gs a context 
  /// @param index_player the player to get probabilities for
  ///
  /// @return probability distribution tuple
  // ----------------------------------------------------------------------
  virtual tuple<double, double>
  getFoldCallProbabilities(const FContext &gs) const = 0;

  // ----------------------------------------------------------------------
  /// @brief   gets a specific probability for an action.
  ///
  /// @param action action to lookup
  /// @param phase to look action up in
  /// @param betting_round to look action up in
  ///
  /// @return probability of action
  // ----------------------------------------------------------------------
  virtual double
  get_action_probability(const Action &action, const PhaseType::Enum &phase,
                         const unsigned &betting_round) const = 0;

  // ----------------------------------------------------------------------
  /// @brief   duplicate current model
  ///
  /// @return clone of current model
  // ----------------------------------------------------------------------
  virtual IModel *clone() const = 0;
};
}

#endif
