#include "avformat/MuxerImpl.h"

#include "public/time/Timestamp.h"
#include "public/utils/Logger.h"

#include "avcodec/AVPacketImpl.h"
#include "utils/exception.h"

#include <sstream>

namespace libffmpegxx {
namespace utils {
extern AVDictionary *toAVDictionary(AVOptions const &options);
}

namespace avformat {
extern void addStreams(AVFormatContext *ctx,
                       std::map<int, StreamInfo> const &streamsInfo);

namespace {
std::string buildDebugInfo(avcodec::IAVPacket *packet) {
  std::stringstream ss;
  ss << "Packet data:";
  ss << "\n\tType: " << static_cast<int>(packet->getContentType());
  ss << "\n\tSize: " << packet->getSize();
  ss << "\n\tStream idx: " << packet->getStreamIndex();
  ss << "\n\tPTS: " << packet->getPts().value();
  ss << "\n\tDTS: " << packet->getDts().value();
  ss << "\n\tTimebase: " << packet->getTimebase().toString();
  ss << "\n\tDuration: " << packet->getDuration();

  return ss.str();
}
} // namespace

IMuxer *MuxerFactory::create(const MediaInfo &mediaInfo) {
  return new MuxerImpl(mediaInfo);
}

MuxerImpl::MuxerImpl(MediaInfo const &mediaInfo) : m_mediaInfo(mediaInfo) {}

MuxerImpl::~MuxerImpl() { this->MuxerImpl::close(); }

void MuxerImpl::open(utils::AVOptions const &options) {
  if (m_formatContext) {
    LOG_WARN("Trying to open muxer for " + m_mediaInfo.uri +
             " but it is already opened");
    return;
  }

  std::lock_guard<std::mutex> l(m_ioMutex);

  int error = avformat_alloc_output_context2(&m_formatContext, nullptr,
                                             m_mediaInfo.format.c_str(),
                                             m_mediaInfo.uri.c_str());
  if (!m_formatContext) {
    LOG_FATAL_FFMPEG_ERR(
        "Error while allocating output context for " + m_mediaInfo.uri, error);
  }

  try {
    addStreams(m_formatContext, m_mediaInfo.streamsInfo);
    LOG_DEBUG("Streams added to muxer " + m_mediaInfo.uri);
  } catch (std::runtime_error const &error) {
    close();
    throw std::runtime_error("Error while adding streams to " +
                             m_mediaInfo.uri + ": " + error.what());
  }

  auto opts = utils::toAVDictionary(options);

  error = avio_open2(&m_formatContext->pb, m_mediaInfo.uri.c_str(),
                     AVIO_FLAG_WRITE, nullptr, &opts);
  if (error < 0) {
    LOG_FATAL_FFMPEG_ERR(
        "Error while opening output for " + m_mediaInfo.uri, error);
  }

  LOG_DEBUG("Muxer opened successfully for " + m_mediaInfo.uri);

  av_dump_format(m_formatContext, 0, m_mediaInfo.uri.c_str(), true);

  error = avformat_write_header(m_formatContext, NULL);
  if (error < 0) {
    LOG_FATAL_FFMPEG_ERR(
        "Error while writing header for " + m_mediaInfo.uri, error);
  }

  LOG_DEBUG("Header written output for " + m_mediaInfo.uri);
}

void MuxerImpl::close() {
  std::lock_guard<std::mutex> l(m_ioMutex);

  if (!m_formatContext) {
    LOG_WARN("Trying to close muxer for " + m_mediaInfo.uri +
             " but it is already closed");
    return;
  }

  LOG_INFO("Closing muxer to " + m_mediaInfo.uri);

  if (m_formatContext->pb) {
    avio_flush(m_formatContext->pb);

    LOG_DEBUG("Writing trailer to " + m_mediaInfo.uri);

    const int error = av_write_trailer(m_formatContext);
    if (error < 0) {
      LOG_FATAL_FFMPEG_ERR(
          "Error while writing trailer for " + m_mediaInfo.uri, error);
    }

    LOG_DEBUG("Closing I/O to " + m_mediaInfo.uri);

    avio_close(m_formatContext->pb);
  } else {
    LOG_ERROR("No I/O context found while closing muxer to " + m_mediaInfo.uri);
  }

  avformat_free_context(m_formatContext);
}

void MuxerImpl::write(avcodec::IAVPacket *packet) {
  if (!m_formatContext) {
    throw std::runtime_error("Muxer to " + m_mediaInfo.uri + " not opened yet");
  }

  auto const debugInfo = buildDebugInfo(packet);

  auto const packetImpl = reinterpret_cast<avcodec::AVPacketImpl *>(packet);
  if (!packetImpl) {
    throw std::runtime_error("Could not handle packet for " + m_mediaInfo.uri +
                             ". " + debugInfo);
  }

  auto const pts = packetImpl->getPts();
  auto const dts = packetImpl->getDts();

  std::lock_guard<std::mutex> l(m_ioMutex);

  auto const streamTb =
      m_formatContext->streams[packetImpl->getStreamIndex()]->time_base;
  auto const tb = time::Timebase({streamTb.num, streamTb.den});

  packetImpl->setTimebase(tb);
  packetImpl->setTimestamp(pts.toTimebase(tb), dts.toTimebase(tb));

  LOG_DEBUG("Writing packet to " + m_mediaInfo.uri + ". " + debugInfo);

  int const error =
      av_write_frame(m_formatContext, packetImpl->getWrappedPacket());
  if (error < 0) {
    LOG_FATAL_FFMPEG_ERR("Error while writing packet for " +
                                     m_mediaInfo.uri + ". " + debugInfo,
                                 error);
  }
}
}; // namespace avformat
}; // namespace libffmpegxx
