#pragma once

#include <stdint.h>

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
   * @param ts The timestamp as an integer value. Units based on timebase.
   * @param tb The timebase of the timestamp.
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
   * @return the timestamp value. Units based on timebase.
   */
  int64_t value() const;

private:
  int64_t _ts{0};
  Timebase _tb{0, 0};
};
}; // namespace time
}; // namespace libffmpegxx
