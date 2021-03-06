#pragma once

#include "../utils/AVOptions.h"
#include "MediaInfo.h"

#include <string>
#include <unordered_map>

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
   * @param options Open options. Optional
   * @return the multimedia info from the opened input.
   * @throws if there's an error while opening the input.
   */
  virtual MediaInfo open(utils::AVOptions const &options = {}) = 0;

  /**
   * @brief Closes the demuxer.
   */
  virtual void close() = 0;

  /**
   * @brief Reads a packet (if available) of content from the input. The packet
   * can belongs to any of the streams the input contains.
   * @param packet The AVPacket where the data will be stored.
   * @return FFmpeg API error code.
   * @note The given packet will be cleared always before actually trying to
   * read any content. If there's an issue reading the content it will remain
   * empty.
   */
  virtual int read(avcodec::IAVPacket *packet) = 0;

  /**
   * @return the multimedia info from the opened input.
   * @throws if the input has not been opened before.
   */
  virtual MediaInfo getMediaInfo() const = 0;
};

class DemuxerFactory {
public:
  static IDemuxer *create(std::string const &uri);
};
}; // namespace avformat
}; // namespace libffmpegxx
