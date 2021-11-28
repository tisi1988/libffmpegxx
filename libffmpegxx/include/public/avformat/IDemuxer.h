#pragma once

#include "MediaInfo.h"

#include <string>

namespace libffmpegxx {
namespace avcodec {
class IAVPacket;
};

namespace avformat {
/**
 * @brief The IDemuxer class demuxer the API of a demuxer.
 *
 * A demuxer is an entity that allows the reading of multimedia content from a
 * file, or from a stream among others.
 */
class IDemuxer {
public:
  virtual ~IDemuxer() = default;

  /**
   * @brief Opens the demuxer.
   * @return the multimedia info from the opened input.
   * @throws if there's an error while opening the input.
   */
  virtual MediaInfo open() = 0;

  /**
   * @brief Closes the demuxer.
   */
  virtual void close() = 0;

  /**
   * @brief Reads a packet (if available) of content from the input. The packet
   * can belongs to any of the streams the input contains.
   * @return the read packet
   * @throws if there's an error while reading the content or EOF.
   */
  virtual avcodec::IAVPacket *read() = 0;

  /**
   * @return the multimedia info from the opened input.
   * @throws if the input has not been opened before.
   */
  virtual MediaInfo getMediaInfo() = 0;
};

class DemuxerFactory {
public:
  static IDemuxer *create(std::string const &uri);
};
}; // namespace avformat
}; // namespace libffmpegxx
