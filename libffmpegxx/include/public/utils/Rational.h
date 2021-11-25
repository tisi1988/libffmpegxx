#pragma once

#include <string>

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
  Rational(int num, int den);

  /**
   * @return the rational's numerator.
   */
  int num() const;

  /**
   * @return the rational's denominator.
   */
  int den() const;

  /**
   * @return the ratio as a floating point number.
   */
  double ratio() const;

  /**
   * @return the string representation of the rational.
   */
  std::string toString() const;

private:
  int _num{0};
  int _den{0};
};
} // namespace utils
} // namespace libffmpegxx
