#pragma once

#include "../public/avcodec/IDecoder.h"

namespace libffmpegxx {
namespace avcodec {
class DecoderImpl : public IDecoder {
public:
  explicit DecoderImpl(avformat::StreamInfo const &streamInfo);

  ~DecoderImpl();

  int decode(IAVPacket *packet, avutil::IAVFrame *frame) override;

private:
  AVCodecContext *m_codecCtx{nullptr};
};
}; // namespace avcodec
}; // namespace libffmpegxx
