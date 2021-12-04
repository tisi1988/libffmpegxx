#pragma once

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
  double averageFramerate;

  /**
   * @brief Image format.
   */
  AVPixelFormat format;

  /**
   * @brief Amount of video frames.
   */
  int64_t frameCount;

  /**
   * @brief Video width.
   */
  int width;

  /**
   * @brief Video height.
   */
  int height;

  /**
   * @brief Additional colorspace characteristics.
   */
  AVChromaLocation chromaLocation;
  AVColorPrimaries colorPrimaries;
  AVColorRange colorRange;
  AVColorSpace colorSpace;
  AVColorTransferCharacteristic colorTrc;

  /**
   * @brief The order of the fields in interlaced video.
   */
  AVFieldOrder fieldOrder;

  /**
   * @brief Number of delayed frames.
   */
  int videoDelay;

  /**
   * @brief The aspect ratio (width / height) which a single pixel
   * should have when displayed.
   */
  utils::Rational sampleAspectRatio;
};

/**
 * @brief The DataInfo struct holds the data of a audio stream.
 */
struct AudioInfo {
  /**
   * @brief Audio sample format.
   */
  AVSampleFormat format;

  /**
   * @brief Audio channel count.
   */
  int channelCount;

  /**
   * @brief Audio channel layout.
   */
  int channelLayout;

  /**
   * @brief Duration of an audio frame (in samples).
   */
  int frameSize;

  /**
   * @brief Audio sample rate in Hz.
   */
  int sampleRate;

  /**
   * @brief The number of bits per sample in the codedwords.
   */
  int bitsPerCodedSample;

  /**
   * @brief This is the number of valid bits in each output sample.
   */
  int bitsPerRawSample;

  /**
   * @brief The number of bytes per coded audio frame, required by some
   * formats.
   */
  int blockAlign;

  /**
   * @brief The amount of padding (in samples) inserted by the encoder at
   * the beginning of the audio.
   */
  int initialPadding;

  /**
   * @brief Number of samples to skip after a discontinuity.
   */
  int seekPreroll;

  /**
   * @brief The amount of padding (in samples) appended by the encoder to
   * the end of the audio.
   */
  int trailingPadding;
};

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
   * @brief Data bitrate in bits per second.
   */
  int64_t bitrate;

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
   * @brief Codec profile.
   */
  int profile;

  /**
   * @brief Codec level.
   */
  int level;

  /**
   * @brief Additional information about the codec (corresponds to the AVI
   * FOURCC).
   */
  uint32_t codecTag;

  /**
   * @brief General type of the encoded data.
   */
  AVMediaType codecType;

  /**
   * @brief Extra binary data needed for initializing the decoder,
   * codec-dependent.
   */
  std::vector<uint8_t> extraData;

  /**
   * @brief Size of the extradata content in bytes.
   */
  int extraDataSize;

  /**
   * @brief Contains the stream metadata.
   */
  utils::AVOptions metadata;

  /**
   * @brief Contains the properties of the stream.
   */
  std::variant<VideoInfo, AudioInfo, SubtitleInfo, DataInfo> properties;
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
