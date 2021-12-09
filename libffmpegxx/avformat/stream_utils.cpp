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
void initializeCommonCodecProperties(StreamInfo const &info,
                                     AVCodecParameters *codecPar) {
  codecPar->bit_rate = info.bitrate;
  codecPar->codec_id = info.codecId;
  codecPar->codec_tag = info.codecTag;
  codecPar->codec_type = info.codecType;
  codecPar->level = info.level;
  codecPar->profile = info.profile;
  codecPar->extradata_size = info.extraDataSize;
  codecPar->extradata =
      reinterpret_cast<uint8_t *>(av_malloc(info.extraDataSize));
  memcpy(codecPar->extradata, info.extraData.data(), info.extraDataSize);
}

void initializeVideoCodecProperties(VideoInfo const &info,
                                    AVCodecParameters *codecPar) {
  codecPar->format = info.format;
  codecPar->chroma_location = info.chromaLocation;
  codecPar->color_primaries = info.colorPrimaries;
  codecPar->color_range = info.colorRange;
  codecPar->color_space = info.colorSpace;
  codecPar->color_trc = info.colorTrc;
  codecPar->field_order = info.fieldOrder;
  codecPar->height = info.height;
  codecPar->video_delay = info.videoDelay;
  codecPar->width = info.width;
  codecPar->sample_aspect_ratio = {info.sampleAspectRatio.num(),
                                   info.sampleAspectRatio.den()};
}

void initializeAudioCodecProperties(AudioInfo const &info,
                                    AVCodecParameters *codecPar) {
  codecPar->bits_per_coded_sample = info.bitsPerCodedSample;
  codecPar->bits_per_raw_sample = info.bitsPerRawSample;
  codecPar->block_align = info.blockAlign;
  codecPar->channel_layout = info.channelLayout;
  codecPar->channels = info.channelCount;
  codecPar->format = info.format;
  codecPar->frame_size = info.frameSize;
  codecPar->initial_padding = info.initialPadding;
  codecPar->sample_rate = info.sampleRate;
  codecPar->seek_preroll = info.seekPreroll;
  codecPar->trailing_padding = info.trailingPadding;
}

void initializeSubtitlesCodecProperties(SubtitleInfo const &info,
                                        AVCodecParameters *codecPar) {
  // TODO
}

void initializeDataCodecProperties(DataInfo const &info,
                                   AVCodecParameters *codecPar) {
  // TODO
}

void initializeCodecParameters(AVCodecParameters *codecPar,
                               StreamInfo const &info) {
  if (std::holds_alternative<VideoInfo>(info.properties)) {
    initializeVideoCodecProperties(std::get<VideoInfo>(info.properties),
                                   codecPar);
  } else if (std::holds_alternative<AudioInfo>(info.properties)) {
    initializeAudioCodecProperties(std::get<AudioInfo>(info.properties),
                                   codecPar);
  } else if (std::holds_alternative<SubtitleInfo>(info.properties)) {
    initializeSubtitlesCodecProperties(std::get<SubtitleInfo>(info.properties),
                                       codecPar);
  } else if (std::holds_alternative<DataInfo>(info.properties)) {
    initializeDataCodecProperties(std::get<DataInfo>(info.properties),
                                  codecPar);
  } else {
    throw std::runtime_error("Content type " +
                             std::to_string(static_cast<int>(info.type)) +
                             " not supported");
  }
}

void initializeStreamParameters(AVStream *stream, StreamInfo const &info) {
  initializeCommonCodecProperties(info, stream->codecpar);

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
    initializeCodecParameters(stream->codecpar, info);
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
