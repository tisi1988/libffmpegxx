#include "avcodec/DecoderImpl.h"

#include "public/avcodec/IAVPacket.h"
#include "public/avutil/IAVFrame.h"

#include "avcodec/AVPacketImpl.h"
#include "avutil/AVFrameImpl.h"
#include "utils/LoggerApi.h"

namespace libffmpegxx {
namespace avformat {
extern void initializeCommonCodecProperties(StreamInfo const &info,
                                            AVCodecParameters *codecPar);
}

namespace {
void initCodecContextCodeParameters(avformat::StreamInfo const &info,
                                    AVCodecContext *codecCtx) {
  auto codecPar = avcodec_parameters_alloc();
  initializeCommonCodecProperties(info, codecPar);
  int const error = avcodec_parameters_to_context(codecCtx, codecPar);
  avcodec_parameters_free(&codecPar);
  if (error < 0) {
    LOG_FATAL("Error while initializing decoder: " +
              utils::Logger::avErrorToStr(error));
  }
}
} // namespace

namespace avcodec {
IDecoder *DecoderFactory::create(avformat::StreamInfo const &streamInfo) {
  return new DecoderImpl(streamInfo);
}

DecoderImpl::DecoderImpl(avformat::StreamInfo const &streamInfo) {
  // First find the decoder
  auto const codec = avcodec_find_decoder(streamInfo.codecId);
  if (!codec) {
    LOG_FATAL("No codec found for id " + std::to_string(streamInfo.codecId));
  }

  m_codecCtx = avcodec_alloc_context3(codec);
  if (!m_codecCtx) {
    LOG_FATAL("Could not allocate decoder context for codec ID " +
              std::to_string(streamInfo.codecId));
  }

  initCodecContextCodeParameters(streamInfo, m_codecCtx);

  int const error = avcodec_open2(m_codecCtx, codec, nullptr);
  if (error < 0) {
    LOG_FATAL("Could not open decoder for codec ID " +
              std::to_string(streamInfo.codecId));
  }
}

DecoderImpl::~DecoderImpl() {
  if (m_codecCtx) {
    avcodec_free_context(&m_codecCtx);
  }
}

int DecoderImpl::decode(IAVPacket *packet, avutil::IAVFrame *frame) {
  auto frameImpl = dynamic_cast<avutil::AVFrameImpl *>(frame);
  if (!frameImpl) {
    LOG_FATAL("Error handling frame while decoding");
  }

  auto packetImpl = dynamic_cast<avcodec::AVPacketImpl *>(packet);
  if (!packetImpl) {
    LOG_FATAL("Error handling packet while decoding");
  }

  auto error = avcodec_send_packet(m_codecCtx, packetImpl->getWrappedPacket());
  if (error < 0) {
    LOG_ERROR("Error sending packet to decoder: " +
              utils::Logger::avErrorToStr(error));

    return error;
  }

  error = avcodec_receive_frame(m_codecCtx, frameImpl->getWrappedFrame());
  if (error < 0) {
    auto const msg = "Error receiving frame from decoder: " +
                     utils::Logger::avErrorToStr(error);
    if (error == AVERROR(EAGAIN) || error == AVERROR_EOF) {
      LOG_INFO(msg);
    } else {
      LOG_ERROR(msg);
    }
  }

  return error;
}
} // namespace avcodec
} // namespace libffmpegxx
