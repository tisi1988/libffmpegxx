#include "avcodec/IAVPacket.h"
#include "avformat/IDemuxer.h"
#include "avformat/IMuxer.h"
#include "time/Timestamp.h"
#include "utils/Logger.h"

#include <iostream>
#include <sstream>

std::string toString(libffmpegxx::avformat::VideoInfo const &info) {
  std::stringstream ss;

  ss << "\t\tAverage framerate: " << info.averageFramerate << std::endl;
  ss << "\t\tPixel format: " << info.format << std::endl;
  ss << "\t\tFrame count: " << info.frameCount << std::endl;
  ss << "\t\tWidth: " << info.width << std::endl;
  ss << "\t\tHeight: " << info.height << std::endl;

  return ss.str();
};

std::string toString(libffmpegxx::avformat::AudioInfo const &info) {
  std::stringstream ss;

  ss << "\t\tSample format: " << info.format << std::endl;
  ss << "\t\tChannel count: " << info.channelCount << std::endl;
  ss << "\t\tChannel layout: " << info.channelLayout << std::endl;
  ss << "\t\tFrame size: " << info.frameSize << std::endl;
  ss << "\t\tSamplerate: " << info.sampleRate << std::endl;

  return ss.str();
};

std::string toString(libffmpegxx::avformat::StreamInfo const &info) {
  std::stringstream ss;

  ss << "\tStream ID: " << info.index << std::endl;
  ss << "\tStream type: " << static_cast<int>(info.type) << std::endl;
  ss << "\tCodec ID: " << info.codecId << std::endl;
  ss << "\tCodec name: " << info.codecName << std::endl;
  ss << "\tBitrate: " << info.bitrate << std::endl;
  ss << "\tDuration: " << info.duration.count() << " seconds" << std::endl;
  ss << "\tTimebase: " << info.timebase.toString() << std::endl;
  ss << "\tStart time: " << info.startTime.count() << " seconds" << std::endl;
  ss << "\tProfile: " << info.profile << std::endl;
  ss << "\tLevel: " << info.level << std::endl;

  switch (info.type) {
  case libffmpegxx::avformat::StreamType::VIDEO:
    ss << toString(std::get<libffmpegxx::avformat::VideoInfo>(info.properties));
    break;
  case libffmpegxx::avformat::StreamType::AUDIO:
    ss << toString(std::get<libffmpegxx::avformat::AudioInfo>(info.properties));
    break;
  case libffmpegxx::avformat::StreamType::SUBTITLE:
  case libffmpegxx::avformat::StreamType::DATA:
  case libffmpegxx::avformat::StreamType::NONE:
  default:
    break;
  }

  return ss.str();
}

std::string toString(libffmpegxx::avformat::MediaInfo const &info) {
  std::stringstream ss;

  ss << "URI: " << info.uri << std::endl;
  ss << "Start time: " << info.startTime.count() << " seconds" << std::endl;
  ss << "Duration: " << info.duration.count() << " seconds" << std::endl;
  ss << "Format: " << info.format << std::endl;
  ss << "Format name: " << info.formatLongName << std::endl;
  ss << "Streams:" << std::endl;
  for (auto &&[id, info] : info.streamsInfo) {
    ss << toString(info);
  }

  return ss.str();
}

int main() {
  // Set Logger to stdout
  auto logger = libffmpegxx::utils::Logger::getLogger();
  std::ostream *oStream = &std::cout;
  logger->setOutputStream(oStream);
  logger->setLogLevel(libffmpegxx::utils::LogLevel::INFO);

  auto demuxer = libffmpegxx::avformat::DemuxerFactory::create(
      "/home/ubuntu/Escriptori/test.mp4");

  // You can open with no options
  auto info = demuxer->open();

  // Or create options. DemuxingOptions supports
  // int or std::string values.
  //  libffmpegxx::avformat::AVOptions options;
  //  options.insert({"analyzeduration", "20000000"});
  //  options.insert({"probesize", 20000000});
  //  auto const info = demuxer->open(options);

  std::cout << toString(info) << std::endl;

  // Overwrite URI and format for muxing
  info.uri = "/home/ubuntu/Escriptori/test.ts";
  info.format = "mpegts";

  auto muxer = libffmpegxx::avformat::MuxerFactory::create(info);
  muxer->open({});

  // then, start reading packets
  libffmpegxx::avcodec::IAVPacket *packet =
      libffmpegxx::avcodec::AVPacketFactory::create();
  int error;
  do {
    error = demuxer->read(packet);
    muxer->write(packet);
  } while (error >= 0);

  delete packet;
  delete demuxer;
  delete muxer;
}
