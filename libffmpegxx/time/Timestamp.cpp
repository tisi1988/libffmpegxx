#include "public/time/Timestamp.h"

#include <cmath>

extern "C" {
#include <libavutil/rational.h>
}

namespace libffmpegxx {
namespace utils {
namespace time {
const Timestamp Timestamp::EMPTY = Timestamp(0, utils::time::Timebase(0, 0));

Timestamp::Timestamp(int64_t ts, const Timebase &tb) : _ts(ts), _tb(tb) {}

Timestamp Timestamp::toTimebase(Timebase const &tb) const {
  double const num = _ts * _tb.num() * tb.den();
  auto const den = _tb.den() * tb.num();
  return Timestamp(std::round(num / den), tb);
}

utils::time::Seconds Timestamp::toSeconds() const {
  if (_ts <= 0 || _tb.num() == 0 || _tb.den() == 0) {
    return utils::time::Seconds{0};
  }

  return utils::time::Seconds{static_cast<double>(_ts * _tb.num()) / _tb.den()};
}

int64_t Timestamp::value() const { return _ts; }

}; // namespace time
}; // namespace utils
}; // namespace libffmpegxx
