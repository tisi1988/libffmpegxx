#pragma once

#include "public/avformat/IMuxer.h"

extern "C" {
#include <libavformat/avformat.h>
}

namespace libffmpegxx {
namespace avformat {
class MuxerImpl : public IMuxer {
public:
  explicit MuxerImpl(MediaInfo const &mediaInfo);
  ~MuxerImpl() override;
  void open(utils::AVOptions const &options = {}) override;
  void close() override;
  void write(avcodec::IAVPacket *packet) override;

private:
  AVFormatContext *m_formatContext{nullptr};
  MediaInfo m_mediaInfo;
};
}; // namespace avformat
}; // namespace libffmpegxx
