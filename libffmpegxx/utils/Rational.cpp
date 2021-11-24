#include "public/utils/Rational.h"

extern "C" {
#include <libavutil/rational.h>
}

namespace libffmpegxx {
namespace utils {
Rational::Rational(int num, int den) : _num(num), _den(den) {}

int Rational::num() const { return _num; }

int Rational::den() const { return _den; }

double Rational::ratio() const { return av_q2d(AVRational{_num, _den}); }

std::string Rational::toString() const {
  return "{" + std::to_string(_num) + ", " + std::to_string(_den) + "}";
}

} // namespace utils
} // namespace libffmpegxx
