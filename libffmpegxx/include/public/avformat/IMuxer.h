#pragma once

#include "../utils/AVOptions.h"
#include "MediaInfo.h"

namespace libffmpegxx {
namespace avcodec {
class IAVPacket;
};

namespace avformat {
/**
 * @brief The IMuxer class muxer the API of a muxer.
 *
 * A muxer is an entity that allows the writing of multimedia content to a
 * file, or to a stream among others.
 */
class IMuxer {
public:
  virtual ~IMuxer() = default;

  /**
   * @brief Opens the muxer.
   * @param options Open options. Optional
   * @throws if there's an error while opening the output.
   */
  virtual void open(utils::AVOptions const &options = {}) = 0;

  /**
   * @brief Closes the muxer.
   */
  virtual void close() = 0;

  /**
   * @brief Writes a packet into the output.
   * @note This method already transforms the timestamps of the packet
   * into the timebase of the stream where it will be written to.
   * @param packet The AVPacket to write.
   */
  virtual void write(avcodec::IAVPacket *packet) = 0;
};

class MuxerFactory {
public:
  static IMuxer *create(MediaInfo const &mediaInfo);
};
}; // namespace avformat
}; // namespace libffmpegxx
