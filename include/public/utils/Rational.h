#pragma once

extern "C" {
#include <libavutil/rational.h>
}

namespace libffmpegxx {
namespace utils {
/**
 * @brief The Rational class implements a rational number.
 */
class Rational {
public:
  /**
   * @brief Rational constructor.
   * @param num Numerator.
   * @param den Denominator.
   */
  Rational(int num, int den) : m_rational({num, den}) {}

  /**
   * @return the rational's numerator.
   */
  int64_t num() const { return m_rational.num; }

  /**
   * @return the rational's denominator.
   */
  int64_t den() const { return m_rational.den; }

  /**
   * @return the ratio as a floating point number.
   */
  double ratio() const { return av_q2d(m_rational); }

private:
  ::AVRational m_rational;
};
} // namespace utils
} // namespace libffmpegxx
