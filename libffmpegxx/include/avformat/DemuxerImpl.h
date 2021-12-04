#pragma once

#include "../public/avcodec/IAVPacket.h"
#include "../public/avformat/IDemuxer.h"
#include "../public/avformat/MediaInfo.h"

#include <mutex>
#include <string>

extern "C" {
#include <libavformat/avformat.h>
}

namespace libffmpegxx {
namespace avcodec {
class AVPacketImpl;
};

namespace avformat {
class DemuxerImpl : public IDemuxer {
public:
  explicit DemuxerImpl(std::string const &uri);
  ~DemuxerImpl() override;
  MediaInfo open(utils::AVOptions const &options = {}) override;
  void close() override;
  int read(avcodec::IAVPacket *packet) override;
  MediaInfo getMediaInfo() const override;

private:
  avformat::StreamType getStreamType(int streamIdx) const;

  std::string m_uri;
  AVFormatContext *m_formatContext{nullptr};

  std::mutex m_ioMutex;
};
}; // namespace avformat
}; // namespace libffmpegxx
