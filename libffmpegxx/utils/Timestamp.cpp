#include "public/utils/time/Timestamp.h"

extern "C" {
#include <libavutil/rational.h>
}

namespace libffmpegxx {
namespace utils {
namespace time {
Timestamp::Timestamp(int64_t ts, const Timebase &tb) : _ts(ts), _tb(tb) {}

Timestamp Timestamp::toTimebase(Timebase const &tb) const {
  auto const num = _ts * _tb.num() * tb.den();
  auto const den = _tb.den() * tb.num();
  return Timestamp(num / den, tb);
}

utils::time::Seconds Timestamp::toSeconds() const {
  if (_ts <= 0 || _tb.num() == 0 || _tb.den() == 0) {
    return utils::time::Seconds{0};
  }

  return utils::time::Seconds{_ts * 1'000'000LL * _tb.den() / _tb.num()};
}
}; // namespace time
}; // namespace utils
}; // namespace libffmpegxx
