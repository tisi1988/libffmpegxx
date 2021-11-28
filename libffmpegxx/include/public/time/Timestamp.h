#pragma once

#include <stdint.h>

#include "../time/Timebase.h"
#include "time_defs.h"

namespace libffmpegxx {
namespace time {

/**
 * Define a timestamp based on a timebase (i.e: stream timebase).
 */
class Timestamp {
public:
  static const Timestamp EMPTY;

  /**
   * @brief Timestamp constructor.
   * @param ts The timestamp as a positive integer. Units based on timebase.
   * @param tb The timebase of the timestamp.
   * @throws if the timestamp is < 0.
   */
  Timestamp(int64_t ts, Timebase const &tb);

  /**
   * @brief Computes the equivalent of this timestamp based on another timebase.
   * @param tb The timebase to transform to.
   * @return the new timestamp based on the given timebase.
   */
  Timestamp toTimebase(Timebase const &tb) const;

  /**
   * @return the timestamp value in seconds.
   */
  Seconds toSeconds() const;

  /**
   * @brief Set a new timestamp value.
   * @param seconds The new timestamp in seconds.
   */
  void set(int64_t const &ts);

  /**
   * @brief Computes the new timestamp value from given seconds using
   * the internal Timebase.
   * @param seconds The new timestamp in seconds.
   * @note Notice the resulting timestamp value could be wrong if the
   * given value is not an integer. However, computed result will
   * be rounded to next integer value.
   */
  void set(time::Seconds const &seconds);

  /**
   * @return the timestamp value. Units based on timebase.
   */
  int64_t value() const;

  /**
   * @return the Timestamp's Timebase.
   */
  time::Timebase getTimebase() const;

private:
  int64_t m_ts{0};
  Timebase m_tb{0, 0};
};
}; // namespace time
}; // namespace libffmpegxx
