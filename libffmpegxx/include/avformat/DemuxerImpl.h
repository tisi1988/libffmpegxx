#pragma once

#include "../public/avcodec/IAVPacket.h"
#include "../public/avformat/IDemuxer.h"
#include "../public/avformat/MediaInfo.h"

#include <string>

extern "C" {
#include <libavformat/avformat.h>
}

namespace libffmpegxx {
namespace avcodec {
class IAVPacket;
};

namespace avformat {
class DemuxerImpl : public IDemuxer {
public:
  explicit DemuxerImpl(std::string const &uri);

  ~DemuxerImpl();

  MediaInfo open() override;
  void close() override;
  int read(avcodec::IAVPacket &packet) override;
  MediaInfo getMediaInfo() override;

private:
  avformat::StreamType getStreamType(int streamIdx) const;

  std::string m_uri;
  AVFormatContext *m_formatContext{nullptr};
  AVPacket *m_readingPacket{nullptr};
};
}; // namespace avformat
}; // namespace libffmpegxx
