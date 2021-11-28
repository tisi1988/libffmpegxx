#include "public/time/Timebase.h"

#include <stdexcept>

namespace libffmpegxx {
namespace time {
Timebase::Timebase() : Rational(1, 1) {}

Timebase::Timebase(int num, int den) {
  if (num <= 0) {
    throw std::runtime_error("Timebase numerator cannot be <= 0");
  }

  if (den <= 0) {
    throw std::runtime_error("Timebase denominator cannot be <= 0");
  }

  Rational(num, den);
}

bool Timebase::operator==(Timebase const &other) const {
  return Rational::operator==(other);
}

bool Timebase::operator!=(Timebase const &other) const {
  return Rational::operator!=(other);
}
}; // namespace time
}; // namespace libffmpegxx
