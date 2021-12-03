#include "avcodec/IAVPacket.h"
#include "avformat/IDemuxer.h"
#include "time/Timestamp.h"
#include "utils/Logger.h"

#include <iostream>
#include <sstream>

std::string toString(libffmpegxx::avcodec::IAVPacket *p) {
  // clang-format off
    std::stringstream ss;
    ss << "Packet data:";
    ss << "\n\tType: " << static_cast<int>(p->getContentType());
    ss << "\n\tSize: " << p->getSize();
    ss << "\n\tStream idx: " << p->getStreamIndex();
    ss << "\n\tPTS: " << p->getPts().value();
    ss << "\n\tDTS: " << p->getDts().value();
    ss << "\n\tTimebase: " << p->getTimebase().toString();
    ss << "\n\tDuration: " << p->getDuration();
  // clang-format on
  return ss.str();
}

std::string toString(libffmpegxx::avformat::StreamBaseInfo const &info) {
  std::stringstream ss;

  ss << "\t\tCodec ID: " << info.codecId << std::endl;
  ss << "\t\tCodec name: " << info.codecName << std::endl;
  ss << "\t\tBitrate: " << info.bitrate << std::endl;
  ss << "\t\tDuration: " << info.duration.count() << " seconds" << std::endl;
  ss << "\t\tTimebase: " << info.timebase.toString() << std::endl;
  ss << "\t\tStart time: " << info.startTime.count() << " seconds" << std::endl;
  ss << "\t\tProfile: " << info.profile << std::endl;
  ss << "\t\tLevel: " << info.level << std::endl;

  return ss.str();
}

std::string toString(libffmpegxx::avformat::VideoInfo const &info) {
  std::stringstream ss;

  ss << toString(static_cast<libffmpegxx::avformat::StreamBaseInfo>(info));
  ss << "\t\tAverage framerate: " << info.averageFramerate << std::endl;
  ss << "\t\tPixel format: " << info.format << std::endl;
  ss << "\t\tFrame count: " << info.frameCount << std::endl;
  ss << "\t\tWidth: " << info.width << std::endl;
  ss << "\t\tHeight: " << info.height << std::endl;

  return ss.str();
};

std::string toString(libffmpegxx::avformat::AudioInfo const &info) {
  std::stringstream ss;

  ss << toString(static_cast<libffmpegxx::avformat::StreamBaseInfo>(info));
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

  switch (info.type) {
  case libffmpegxx::avformat::StreamType::VIDEO:
    ss << toString(std::get<libffmpegxx::avformat::VideoInfo>(info.properties));
    break;
  case libffmpegxx::avformat::StreamType::AUDIO:
    ss << toString(std::get<libffmpegxx::avformat::AudioInfo>(info.properties));
    break;
  case libffmpegxx::avformat::StreamType::SUBTITLE:
    ss << toString(
        std::get<libffmpegxx::avformat::SubtitleInfo>(info.properties));
    break;
  case libffmpegxx::avformat::StreamType::DATA:
    ss << toString(std::get<libffmpegxx::avformat::DataInfo>(info.properties));
    break;
  case libffmpegxx::avformat::StreamType::NONE:
  default:;
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
  auto const info = demuxer->open();

  // Or create options. DemuxingOptions supports
  // int or std::string values.
  //  libffmpegxx::avformat::DemuxingOptions options;
  //  options.insert({"analyzeduration", "20000000"});
  //  options.insert({"probesize", 20000000});
  //  auto const info = demuxer->open(options);

  std::cout << toString(info) << std::endl;

  // then, start reading packets
  libffmpegxx::avcodec::IAVPacket *packet =
      libffmpegxx::avcodec::AVPacketFactory::create();
  int error;
  do {
    error = demuxer->read(packet);
    if (error >= 0) {
      std::cout << toString(packet) << std::endl;
    }
  } while (error >= 0);

  delete packet;
  delete demuxer;
}
