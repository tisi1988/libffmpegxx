#pragma once

#include "../time/time_defs.h"

#include <map>
#include <string>

namespace libffmpegxx {
namespace avformat {
struct StreamInfo {
  /**
   * @brief The Type enum defines the possible stream types.
   */
  enum class Type { NONE = 0, VIDEO, AUDIO, SUBTITLE, DATA };

  /**
   * @brief Stream index.
   */
  int index;

  /**
   * @brief Stream type.
   */
  Type type;

  // Codec, timebase, ...
};

/**
 * @brief The MediaInfo struct contains all the information
 * about a media source/destination.
 */
struct MediaInfo {
  /**
   * @brief Information of each stream.
   */
  std::map<int, StreamInfo> streamsInfo;

  /**
   * @brief The media URI (file path, streaming URL, ...)
   */
  std::string uri;

  /**
   * @brief Duration of the stream.
   */
  time::Seconds duration;

  /**
   * @brief Short format name, like: "mov,mp4,m4a,3gp,3g2,mj2".
   */
  std::string format;

  /**
   * @brief Full format name, like: "QuickTime/MPEG-4/Motion JPEG 2000 format".
   */
  std::string formatLongName;

  /**
   * @brief Media start time.
   */
  time::Seconds startTime;

  /**
   * @brief Media size in bytes.
   */
  size_t sizeBytes;
};
}; // namespace avformat
}; // namespace libffmpegxx
