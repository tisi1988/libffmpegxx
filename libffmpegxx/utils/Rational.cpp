#include "public/utils/Rational.h"

extern "C" {
#include <libavutil/rational.h>
}

namespace libffmpegxx {
namespace utils {
Rational::Rational(int num, int den) : m_num(num), m_den(den) {}

int Rational::num() const { return m_num; }

int Rational::den() const { return m_den; }

double Rational::ratio() const { return av_q2d(AVRational{m_num, m_den}); }

std::string Rational::toString() const {
  return "{" + std::to_string(m_num) + ", " + std::to_string(m_den) + "}";
}

bool Rational::operator==(Rational const &other) const {
  return m_num == other.m_num && m_den == other.m_den;
}

bool Rational::operator!=(Rational const &other) const {
  return !(*this == other);
}
} // namespace utils
} // namespace libffmpegxx
