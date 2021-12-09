#pragma once

#include "../avformat/MediaInfo.h"

namespace libffmpegxx {
namespace avutil {
class IAVFrame;
}
namespace avcodec {
class IAVPacket;

/**
 * @brief The IDecoder class defines the decoder features.
 */
class IDecoder {
public:
  virtual ~IDecoder() = default;

  /**
   * @brief Tries to decode an encoded data packet.
   * @param packet The packet to decode.
   * @param frame The frame to store the output data.
   * @return FFmpeg API error code.
   * @note Some decoders may fail to return decoded content
   * on the first bunch of packets.
   * @note To flush the decoder simply feed empty IAVPackets till
   * the returned error code is not 0.
   */
  virtual int decode(IAVPacket *packet, avutil::IAVFrame *frame) = 0;
};

/**
 * @brief The DecoderFactory class creates decoders.
 */
class DecoderFactory {
public:
  /**
   * @brief Creates a decoder for a given stream.
   * @param streamInfo The info about the stream to decode.
   * @return a new decoder for the given stream.
   */
  static IDecoder *create(avformat::StreamInfo const &streamInfo);
};
}; // namespace avcodec
}; // namespace libffmpegxx
