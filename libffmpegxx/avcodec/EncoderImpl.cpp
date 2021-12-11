#include "avcodec/EncoderImpl.h"

#include "public/avcodec/IAVPacket.h"
#include "public/avutil/IAVFrame.h"

#include "avcodec/AVPacketImpl.h"
#include "avutil/AVFrameImpl.h"
#include "utils/LoggerApi.h"
#include "utils/exception.h"

namespace libffmpegxx {
namespace avcodec {
IEncoder *EncoderFactory::create(avformat::StreamInfo const &streamInfo) {
  return new EncoderImpl(streamInfo);
}

EncoderImpl::EncoderImpl(const avformat::StreamInfo &streamInfo) {
  auto codec = avcodec_find_encoder(streamInfo.codecId);
  if (!codec) {
    LOG_FATAL("Could not find an encoder for codec id " +
              std::to_string(streamInfo.codecId));
  }

  m_codecCtx = avcodec_alloc_context3(codec);
  if (!m_codecCtx) {
    LOG_FATAL("Could not create encoder context for codec id " +
              std::to_string(streamInfo.codecId));
  }

  avcodec_parameters_to_context(m_codecCtx, *streamInfo.codecPar);

  if (std::holds_alternative<avformat::VideoInfo>(streamInfo.properties)) {
    auto const videoProps =
        std::get<avformat::VideoInfo>(streamInfo.properties);
    m_codecCtx->time_base = {videoProps.averageFramerate.num(),
                             videoProps.averageFramerate.den()};
  }

  int const error = avcodec_open2(m_codecCtx, codec, nullptr);
  if (error < 0) {
    LOG_FATAL_FFMPEG_ERR("Could not open decoder for codec ID " +
                             std::to_string(streamInfo.codecId),
                         error);
  }

  m_tb = streamInfo.timebase;
  m_type = streamInfo.type;
}

EncoderImpl::~EncoderImpl() {
  if (m_codecCtx) {
    avcodec_free_context(&m_codecCtx);
  }
}

int EncoderImpl::encode(avutil::IAVFrame *frame, IAVPacket *packet) {
  auto frameImpl = dynamic_cast<avutil::AVFrameImpl *>(frame);
  if (!frameImpl) {
    LOG_FATAL("Error handling frame while decoding");
  }

  auto packetImpl = dynamic_cast<avcodec::AVPacketImpl *>(packet);
  if (!packetImpl) {
    LOG_FATAL("Error handling packet while decoding");
  }

  auto error = avcodec_send_frame(m_codecCtx, frameImpl->getWrappedFrame());
  if (error != 0) {
    LOG_ERROR("Error sending frame to encoder " +
              utils::Logger::avErrorToStr(error));
  }

  error = avcodec_receive_packet(m_codecCtx, packetImpl->getWrappedPacket());
  if (error != 0) {
    if (error != AVERROR(EAGAIN)) {
      LOG_ERROR("Error retrieving packet from from encoder " +
                utils::Logger::avErrorToStr(error));
    }
    packet->clear();
  }

  return error;
}

int EncoderImpl::flush(std::vector<avcodec::IAVPacket *> &flushedPackets) {
  auto error = avcodec_send_frame(m_codecCtx, nullptr);
  if (error < 0) {
    LOG_ERROR("Error sending packet to encoder while flushing: " +
              utils::Logger::avErrorToStr(error));

    return error;
  }

  do {
    AVPacket *p = av_packet_alloc();
    error = avcodec_receive_packet(m_codecCtx, p);
    if (error < 0) {
      auto const msg = "Error receiving packet from encoder while flushing: " +
                       utils::Logger::avErrorToStr(error);
      if (error == AVERROR_EOF) {
        LOG_INFO(msg);
      } else {
        LOG_ERROR(msg);
      }
      av_packet_free(&p);
    } else {
      flushedPackets.push_back(new avcodec::AVPacketImpl(p, m_tb, m_type));
    }
  } while (error == 0);

  return error;
}
} // namespace avcodec
} // namespace libffmpegxx
