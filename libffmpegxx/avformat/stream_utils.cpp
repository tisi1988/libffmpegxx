#include "public/avformat/MediaInfo.h"

#include <stdexcept>

extern "C" {
#include <libavformat/avformat.h>
}

namespace libffmpegxx {
namespace utils {
extern AVDictionary *toAVDictionary(AVOptions const &options);
}

namespace avformat {
void initializeStreamParameters(AVStream *stream, StreamInfo const &info) {
  avcodec_parameters_copy(stream->codecpar, *info.codecPar);

  stream->time_base = {info.timebase.num(), info.timebase.den()};
  stream->start_time = info.startTime.count();
  stream->metadata = utils::toAVDictionary(info.metadata);
}

void addStreams(AVFormatContext *ctx,
                std::map<int, StreamInfo> const &streamsInfo) {
  for (auto &&[streamIdx, info] : streamsInfo) {
    auto const stream = avformat_new_stream(ctx, nullptr);
    if (stream == nullptr) {
      throw std::runtime_error("Error while adding stream " +
                               std::to_string(streamIdx));
    }

    stream->id = ctx->nb_streams - 1;
    initializeStreamParameters(stream, info);

// Some de/muxers still use the AV'Codec'codec' attribute.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    avcodec_parameters_to_context(stream->codec, stream->codecpar);
#pragma GCC diagnostic pop
  }
}
} // namespace avformat
} // namespace libffmpegxx
