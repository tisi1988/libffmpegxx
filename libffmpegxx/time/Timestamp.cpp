#include "public/time/Timestamp.h"

#include <cmath>
#include <stdexcept>

extern "C" {
#include <libavutil/rational.h>
}

namespace libffmpegxx {
namespace time {
const Timestamp Timestamp::EMPTY = Timestamp(0, time::Timebase());

Timestamp::Timestamp(int64_t ts, const Timebase &tb) : m_tb(tb) {
  if (ts < 0) {
    throw std::runtime_error("Timestamp cannot be negative");
  }
  m_ts = ts;
}

Timestamp Timestamp::toTimebase(Timebase const &tb) const {
  double const num = m_ts * m_tb.num() * tb.den();
  auto const den = m_tb.den() * tb.num();
  return Timestamp(std::round(num / den), tb);
}

time::Seconds Timestamp::toSeconds() const {
  if (m_ts <= 0 || m_tb.num() == 0 || m_tb.den() == 0) {
    return time::Seconds{0};
  }

  return time::Seconds{static_cast<double>(m_ts * m_tb.num()) / m_tb.den()};
}

void Timestamp::set(int64_t const &ts) { m_ts = ts; }

void Timestamp::set(time::Seconds const &seconds) {
  if (seconds.count() < 0) {
    throw std::runtime_error("Timestamp cannot be negative");
  }

  m_ts = std::ceil(seconds.count() * m_tb.den() / m_tb.num());
}

int64_t Timestamp::value() const { return m_ts; }

time::Timebase Timestamp::getTimebase() const { return m_tb; }

}; // namespace time
}; // namespace libffmpegxx
