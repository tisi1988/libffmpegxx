#pragma once

#include <chrono>

namespace libffmpegxx {
namespace time {
/**
 * Define seconds using std::chrono with double precision.
 */
using Seconds = std::chrono::duration<double>;
}; // namespace time
}; // namespace libffmpegxx
