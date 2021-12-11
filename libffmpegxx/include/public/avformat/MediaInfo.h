#pragma once

#include "../avcodec/AVCodecPar.h"
#include "../time/Timebase.h"
#include "../time/time_defs.h"
#include "../utils/AVOptions.h"
#include "../utils/Rational.h"

#include <map>
#include <string>
#include <variant>
#include <vector>

extern "C" {
#include <libavcodec/avcodec.h>
}

namespace libffmpegxx {
namespace avformat {
/**
 * @brief The StreamType enum defines the possible stream data types.
 */
enum class StreamType { NONE = 0, VIDEO, AUDIO, SUBTITLE, DATA };

/**
 * @brief The DataInfo struct holds the data of a video stream.
 */
struct VideoInfo {
  /**
   * @brief Average framerate in frames per second.
   */
  utils::Rational averageFramerate;

  /**
   * @brief Amount of video frames.
   */
  int64_t frameCount;
};

/**
 * @brief The DataInfo struct holds the data of a audio stream.
 */
struct AudioInfo {};

/**
 * @brief The DataInfo struct holds the data of a subtitle stream.
 */
struct SubtitleInfo {};

/**
 * @brief The DataInfo struct holds the data of a data stream.
 */
struct DataInfo {};

struct StreamInfo {
  /**
   * @brief Stream index.
   */
  int index;

  /**
   * @brief Stream type.
   */
  StreamType type;

  /**
   * @brief Codec id.
   */
  AVCodecID codecId;

  /**
   * @brief Codec name.
   */
  std::string codecName;

  /**
   * @brief Stream duration in seconds.
   */
  time::Seconds duration;

  /**
   * @brief Stream timebase.
   */
  time::Timebase timebase;

  /**
   * @brief Stream start time.
   */
  time::Seconds startTime;

  /**
   * @brief Contains the stream metadata.
   */
  utils::AVOptions metadata;

  /**
   * @brief Contains the properties of the stream.
   */
  std::variant<VideoInfo, AudioInfo, SubtitleInfo, DataInfo> properties;

  /**
   * @brief Contains the properties of the codec.
   */
  avcodec::AVCodecPar codecPar;
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
   * @brief Full format name, like: "QuickTime/MPEG-4/Motion JPEG 2000
   * format".
   */
  std::string formatLongName;

  /**
   * @brief Media start time.
   */
  time::Seconds startTime;
};
}; // namespace avformat
}; // namespace libffmpegxx
