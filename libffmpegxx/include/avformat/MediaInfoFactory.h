#pragma once

#include "../public/avformat/MediaInfo.h"

struct AVFormatContext;

namespace libffmpegxx {
namespace avformat {
/**
 * @brief The MediaInfoFactory class is a builder class
 * for MediaInfo data structure.
 */
class MediaInfoFactory {
public:
  /**
   * @brief Build a MediaInfo containing the information from a media.
   * @param ctx The AVFormatContext pointer to the media.
   * @return the MediaInfo data structure instance.
   */
  static MediaInfo build(AVFormatContext *ctx);
};
} // namespace avformat
} // namespace libffmpegxx
