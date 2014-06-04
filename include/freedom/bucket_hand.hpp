/*
 * File:   bucket_hand.h
 * Author: batman
 *
 * Created on August 13, 2013, 11:08 PM
 */

#ifndef BUCKET_HAND_H
#define BUCKET_HAND_H

#include <string>
#include <sstream>
#include <poker/hand.hpp>

namespace freedom {

class BucketHand : public poker::Hand {
public:
  double equity;

  BucketHand(int c1, int c2, double _equity)
      : poker::Hand(c1, c2), equity(_equity) {}

  BucketHand(Hand _hand, double _equity)
      : poker::Hand(_hand), equity(_equity) {}

  ~BucketHand() {}

  bool operator==(const BucketHand &oh) const { return equity == oh.equity; }

  bool operator!=(const BucketHand &oh) const { return !(*this == oh); }

  bool operator<(const BucketHand &oh) const { return equity < oh.equity; }

  bool operator<=(const BucketHand &oh) const { return equity <= oh.equity; }

  bool operator>(const BucketHand &oh) const { return equity > oh.equity; }

  bool operator>=(const BucketHand &oh) const { return equity >= oh.equity; }

  virtual std::string to_str(bool print_equity = false) const {
    std::stringstream str;
    str << Hand::str();
    if (print_equity)
      str << " " << equity;
    return str.str();
  }
};
}

#endif /* BUCKET_HAND_H */

