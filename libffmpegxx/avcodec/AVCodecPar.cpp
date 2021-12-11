#include "public/avcodec/AVCodecPar.h"

namespace libffmpegxx {
namespace avcodec {
AVCodecPar::AVCodecPar() { codecPar = avcodec_parameters_alloc(); }

AVCodecPar::AVCodecPar(AVCodecParameters *p) {
  codecPar = avcodec_parameters_alloc();
  avcodec_parameters_copy(codecPar, p);
}

AVCodecPar::~AVCodecPar() { avcodec_parameters_free(&codecPar); }

AVCodecPar::AVCodecPar(AVCodecPar const &other) : AVCodecPar(other.codecPar) {}

AVCodecParameters *AVCodecPar::operator*() const { return codecPar; }

AVCodecParameters *AVCodecPar::operator->() const { return codecPar; }

AVCodecPar &AVCodecPar::operator=(AVCodecPar const &other) {
  avcodec_parameters_copy(codecPar, other.codecPar);

  return *this;
}
}; // namespace avcodec
}; // namespace libffmpegxx
