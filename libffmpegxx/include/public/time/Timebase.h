#pragma once

#include "../utils/Rational.h"

namespace libffmpegxx {
namespace time {
/**
 * Define a timebase as a rational.
 *
 * A timebase is the way to represent the precision of timestamps in ticks per
 * seconds. For instance, 48KHz audio will have a timebase of {1, 48'000}.
 */
class Timebase : public utils::Rational {
public:
  /**
   * @brief Timebase constructor
   */
  Timebase();

  /**
   * @brief Timebase constructor.
   * @param num Numerator.
   * @param den Denominator.
   * @throws if any argument is <= 0.
   */
  Timebase(int num, int den);

  bool operator==(Timebase const &other) const;
  bool operator!=(Timebase const &other) const;
};
}; // namespace time
}; // namespace libffmpegxx
