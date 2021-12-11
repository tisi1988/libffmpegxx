#pragma once

#include "../public/avcodec/IDecoder.h"

namespace libffmpegxx {
namespace avcodec {
class DecoderImpl : public IDecoder {
public:
  explicit DecoderImpl(avformat::StreamInfo const &streamInfo);

  ~DecoderImpl();

  int decode(IAVPacket *packet, avutil::IAVFrame *frame) override;
  int flush(std::vector<avutil::IAVFrame *> &flushedFrames) override;

private:
  AVCodecContext *m_codecCtx{nullptr};
  time::Timebase m_tb;
};
}; // namespace avcodec
}; // namespace libffmpegxx
