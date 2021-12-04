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

IMuxer *MuxerFactory::create(const MediaInfo &mediaInfo) {
  return new MuxerImpl(mediaInfo);
}

MuxerImpl::MuxerImpl(MediaInfo const &mediaInfo) : m_mediaInfo(mediaInfo) {}

MuxerImpl::~MuxerImpl() { this->MuxerImpl::close(); }

void MuxerImpl::open(utils::AVOptions const &options) {
  if (m_formatContext) {
    throw std::runtime_error("Muxer to " + m_mediaInfo.uri + " already opened");
  }

  std::lock_guard<std::mutex> l(m_ioMutex);

  int error = avformat_alloc_output_context2(&m_formatContext, nullptr,
                                             m_mediaInfo.format.c_str(),
                                             m_mediaInfo.uri.c_str());
  if (!m_formatContext) {
    THROW_FFMPEG_ERR_DESCRIPTION(
        "Error while allocating output context for " + m_mediaInfo.uri, error);
  }

  try {
    addStreams(m_formatContext, m_mediaInfo.streamsInfo);
  } catch (std::runtime_error const &error) {
    close();
    throw std::runtime_error("Error while adding streams to " +
                             m_mediaInfo.uri + ": " + error.what());
  }

  auto opts = utils::toAVDictionary(options);

  error = avio_open2(&m_formatContext->pb, m_mediaInfo.uri.c_str(),
                     AVIO_FLAG_WRITE, nullptr, &opts);
  if (error < 0) {
    THROW_FFMPEG_ERR_DESCRIPTION(
        "Error while opening output for " + m_mediaInfo.uri, error);
  }

  av_dump_format(m_formatContext, 0, m_mediaInfo.uri.c_str(), true);

  error = avformat_write_header(m_formatContext, NULL);
  if (error < 0) {
    THROW_FFMPEG_ERR_DESCRIPTION(
        "Error while writing header for " + m_mediaInfo.uri, error);
  }
}

void MuxerImpl::close() {
  std::lock_guard<std::mutex> l(m_ioMutex);

  if (m_formatContext) {
    if (m_formatContext->pb) {
      avio_flush(m_formatContext->pb);

      const int error = av_write_trailer(m_formatContext);
      if (error < 0) {
        THROW_FFMPEG_ERR_DESCRIPTION(
            "Error while writing trailer for " + m_mediaInfo.uri, error);
      }

      avio_close(m_formatContext->pb);
    }

    avformat_free_context(m_formatContext);
  }
}

void MuxerImpl::write(avcodec::IAVPacket *packet) {
  if (!m_formatContext) {
    throw std::runtime_error("Muxer to " + m_mediaInfo.uri + " not opened yet");
  }

  std::stringstream packetInfo;
  packetInfo << "[IDX=" << packet->getStreamIndex();
  packetInfo << "|PTS=" << packet->getPts().value();
  packetInfo << "|DTS=" << packet->getDts().value();

  auto const packetImpl = reinterpret_cast<avcodec::AVPacketImpl *>(packet);
  if (!packetImpl) {
    throw std::runtime_error("Could not handle packet for " + m_mediaInfo.uri +
                             ". " + packetInfo.str());
  }

  auto const pts = packetImpl->getPts();
  auto const dts = packetImpl->getDts();

  std::lock_guard<std::mutex> l(m_ioMutex);

  auto const streamTb =
      m_formatContext->streams[packetImpl->getStreamIndex()]->time_base;
  auto const tb = time::Timebase({streamTb.num, streamTb.den});

  packetImpl->setTimebase(tb);
  packetImpl->setTimestamp(pts.toTimebase(tb), dts.toTimebase(tb));

  int const error =
      av_write_frame(m_formatContext, packetImpl->getWrappedPacket());
  if (error < 0) {
    THROW_FFMPEG_ERR_DESCRIPTION("Error while writing packet for " +
                                     m_mediaInfo.uri + ". " + packetInfo.str(),
                                 error);
  }
}
}; // namespace avformat
}; // namespace libffmpegxx
