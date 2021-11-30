#pragma once

#include "../public/avformat/IDemuxer.h"

#include <string>

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
};
}; // namespace avformat
}; // namespace libffmpegxx
