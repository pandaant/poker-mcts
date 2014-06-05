#ifndef BUCKET_HAND_H
#define BUCKET_HAND_H

#include <string>
#include <poker/hand.hpp>
#include <ecalc/types.hpp>

namespace freedom {

// ----------------------------------------------------------------------
/// @brief   Wrapper around a normal poker hand. Additionally the equity
///          of the hand is saved to.
// ----------------------------------------------------------------------
class BucketHand : public poker::Hand {
  typedef typename ecalc::card card;

public:
  double equity;

  BucketHand(const card &c1, const card &c2, const double &equity_)
      : poker::Hand(c1, c2), equity(equity_) {}

  BucketHand(const Hand &hand_, const double &equity_)
      : poker::Hand(hand_), equity(equity_) {}

  ~BucketHand() {}

  virtual std::string str(const bool &print_equity = false) const {
    return Hand::str() + (print_equity ? ("\t" + std::to_string(equity)) : "");
  }

  bool operator==(const BucketHand &oh) const { return equity == oh.equity; }
  bool operator!=(const BucketHand &oh) const { return !(*this == oh); }
  bool operator<(const BucketHand &oh) const { return equity < oh.equity; }
  bool operator<=(const BucketHand &oh) const { return equity <= oh.equity; }
  bool operator>(const BucketHand &oh) const { return equity > oh.equity; }
  bool operator>=(const BucketHand &oh) const { return equity >= oh.equity; }
};
}

#endif
