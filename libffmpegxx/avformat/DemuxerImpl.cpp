#include "avformat/DemuxerImpl.h"

#include "avcodec/AVPacketImpl.h"

namespace libffmpegxx {
namespace avformat {
IDemuxer *DemuxerFactory::create(std::string const &uri) {
  // TODO
}

DemuxerImpl::DemuxerImpl(std::string const &uri) {
  // TODO
}

DemuxerImpl::~DemuxerImpl() {
  // TODO
}

MediaInfo DemuxerImpl::open() {
  // TODO
}

void DemuxerImpl::close() {
  // TODO
}

int DemuxerImpl::read(avcodec::IAVPacket &packet) {
}

MediaInfo DemuxerImpl::getMediaInfo() {
  // TODO
}
}; // namespace avformat
}; // namespace libffmpegxx
