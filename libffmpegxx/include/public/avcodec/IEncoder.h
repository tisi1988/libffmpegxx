#pragma once

#include "../avformat/MediaInfo.h"

namespace libffmpegxx {
namespace avutil {
class IAVFrame;
}
namespace avcodec {
class IAVPacket;

/**
 * @brief The IEncoder class defines the encoder features.
 */
class IEncoder {
public:
  virtual ~IEncoder() = default;

  /**
   * @brief Tries to encode a frame.
   * @param frame The frame to encode.
   * @param packet The packet to store the encoded data.
   * @return FFmpeg API error code.
   * @note Some encoders may need several frames before start producing
   * encoded data.
   * @note To flush the encoder simply feed empty IAVFrames till
   * the returned error code is not 0.
   */
  virtual int encode(avutil::IAVFrame *frame, IAVPacket *packet) = 0;

  /**
   * @brief Flushes the encoder getting out any remaining packets.
   * @param flushedPackets List of packets flushed from the encoder.
   * @return FFmpeg API error code.
   */
  virtual int flush(std::vector<avcodec::IAVPacket *> &flushedPackets) = 0;
};

/**
 * @brief The EncoderFactory class creates encoders.
 */
class EncoderFactory {
public:
  /**
   * @brief
   * @return a new encoder.
   */
  static IEncoder *create(avformat::StreamInfo const &streamInfo);
};
}; // namespace avcodec
}; // namespace libffmpegxx
