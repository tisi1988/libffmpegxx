#pragma once

extern "C" {
#include <libavcodec/codec_par.h>
}

namespace libffmpegxx {
namespace avcodec {
/**
 * @brief The IAVPacket class defines the AVPacket features.
 */
struct AVCodecPar {
  AVCodecPar();
  explicit AVCodecPar(AVCodecParameters *p);
  ~AVCodecPar();
  explicit AVCodecPar(AVCodecPar const &other);

  AVCodecParameters *operator*() const;
  AVCodecParameters *operator->() const;
  AVCodecPar &operator=(AVCodecPar const &other);

  AVCodecParameters *codecPar;
};
}; // namespace avcodec
}; // namespace libffmpegxx
