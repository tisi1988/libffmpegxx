#include "avformat/DemuxerImpl.h"

#include "avcodec/AVPacketImpl.h"
#include "utils/LoggerApi.h"
#include "utils/exception.h"

#include <sstream>
#include <vector>

libffmpegxx::avformat::MediaInfo initializeMediaInfo(AVFormatContext *ctx) {
  libffmpegxx::avformat::MediaInfo info;

  // TODO

  return info;
}

namespace libffmpegxx {
namespace avformat {
IDemuxer *DemuxerFactory::create(std::string const &uri) {
  return new DemuxerImpl(uri);
}

DemuxerImpl::DemuxerImpl(std::string const &uri) : m_uri(uri) {
  m_readingPacket = av_packet_alloc();

  if (!m_readingPacket) {
    throw std::runtime_error("Unable to allocate packet for reading media " +
                             m_uri);
  }
}

DemuxerImpl::~DemuxerImpl() { this->DemuxerImpl::close(); }

MediaInfo DemuxerImpl::open() {
  if (m_formatContext) {
    throw std::runtime_error("Trying to open but it is already opened");
  }

  m_formatContext = avformat_alloc_context();
  if (m_formatContext == nullptr) {
    throw std::runtime_error("Error allocating format context.");
  }

  // TODO Implement option handling
  AVDictionary *options{nullptr};

  int error = avformat_open_input(&m_formatContext, m_uri.c_str(), nullptr,
                                  options ? (&options) : nullptr);

  if (error < 0) {
    close();
    THROW_FFMPEG_ERR_DESCRIPTION("Error opening media: " + m_uri, error)
  }

  // Get streams info
  error = avformat_find_stream_info(m_formatContext,
                                    options ? (&options) : nullptr);
  if (error < 0) {
    close();
    THROW_FFMPEG_ERR_DESCRIPTION(
        "Could not open find stream info for media: " + m_uri, error)
  }

  // Dump media info to the log
  av_dump_format(m_formatContext, 0, m_formatContext->url, false);

  m_mediaInfo = initializeMediaInfo(m_formatContext);

  return m_mediaInfo;
}

void DemuxerImpl::close() {
  avformat_close_input(&m_formatContext);
  av_packet_free(&m_readingPacket);
}

int DemuxerImpl::read(avcodec::IAVPacket &packet) {
  packet.clear();

  int const error = av_read_frame(m_formatContext, m_readingPacket);

  if (error < 0) {
    auto const msgError = "Error while reading " + m_uri + ": " +
                          utils::Logger::avErrorToStr(error);
    LOG_WARN(msgError);
    return error;
  }

  auto const tb =
      m_formatContext->streams[m_readingPacket->stream_index]->time_base;
  packet = libffmpegxx::avcodec::AVPacketImpl(
      m_readingPacket, {tb.num, tb.den},
      getStreamType(m_readingPacket->stream_index));

  return error;
}

MediaInfo DemuxerImpl::getMediaInfo() { return m_mediaInfo; }

avcodec::IAVPacket::Type DemuxerImpl::getStreamType(int streamIdx) const {
  static std::vector<AVMediaType> const EXPECTED_TYPES = {
      AVMEDIA_TYPE_VIDEO, AVMEDIA_TYPE_AUDIO, AVMEDIA_TYPE_DATA,
      AVMEDIA_TYPE_SUBTITLE, AVMEDIA_TYPE_ATTACHMENT};

  AVMediaType foundType = AVMEDIA_TYPE_UNKNOWN;
  for (AVMediaType mediaType : EXPECTED_TYPES) {
    int foundIndex = av_find_best_stream(m_formatContext, mediaType, streamIdx,
                                         -1, nullptr, 0);
    if (foundIndex == streamIdx) {
      foundType = mediaType;
    }
  }

  switch (foundType) {
  case AVMEDIA_TYPE_VIDEO:
    return avcodec::IAVPacket::Type::VIDEO;
  case AVMEDIA_TYPE_AUDIO:
    return avcodec::IAVPacket::Type::AUDIO;
  case AVMEDIA_TYPE_DATA:
    return avcodec::IAVPacket::Type::DATA;
  case AVMEDIA_TYPE_SUBTITLE:
    return avcodec::IAVPacket::Type::SUBTITLE;
  default:
    LOG_WARN("Could not find stream type for stream " +
             std::to_string(streamIdx) + " in media " + m_uri);
    return avcodec::IAVPacket::Type::NONE;
  }
}
}; // namespace avformat
}; // namespace libffmpegxx
