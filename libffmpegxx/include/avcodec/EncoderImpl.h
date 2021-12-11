#pragma once

#include "../public/avcodec/IEncoder.h"

namespace libffmpegxx {
namespace avcodec {
class EncoderImpl : public IEncoder {
public:
  explicit EncoderImpl(avformat::StreamInfo const &streamInfo);

  ~EncoderImpl();

  int encode(avutil::IAVFrame *frame, IAVPacket *packet) override;

  int flush(std::vector<avcodec::IAVPacket *> &flushedPackets) override;

private:
  AVCodecContext *m_codecCtx{nullptr};
  time::Timebase m_tb;
  avformat::StreamType m_type;
};
}; // namespace avcodec
}; // namespace libffmpegxx
