#pragma once

#include "../utils/Rational.h"

#include <chrono>

namespace libffmpegxx {
namespace utils {
namespace time {
/**
 * Define seconds using std::chrono with double precision.
 */
using Seconds = std::chrono::duration<double>;

/**
 * Define a timebase as a rational.
 */
using Timebase = utils::Rational;
}; // namespace time
}; // namespace utils
}; // namespace libffmpegxx
