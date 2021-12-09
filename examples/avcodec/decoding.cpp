#include "avcodec/IAVPacket.h"
#include "avcodec/IDecoder.h"
#include "avformat/IDemuxer.h"
#include "avutil/IAVFrame.h"
#include "utils/Logger.h"

#include <iostream>

int main() {
  // Set Logger to stdout
  auto logger = libffmpegxx::utils::Logger::getLogger();
  std::ostream *oStream = &std::cout;
  logger->setOutputStream(oStream);
  logger->setLogLevel(libffmpegxx::utils::LogLevel::INFO);

  // Create and open demuxer
  auto demuxer = libffmpegxx::avformat::DemuxerFactory::create(
      "/home/ubuntu/Escriptori/test.mp4");
  auto info = demuxer->open();

  // Create decoders
  std::map<int, libffmpegxx::avcodec::IDecoder *> decoders;
  for (auto &&[idx, info] : info.streamsInfo) {
    decoders.insert({idx, libffmpegxx::avcodec::DecoderFactory::create(info)});
  }

  // Start reading packets
  libffmpegxx::avcodec::IAVPacket *packet =
      libffmpegxx::avcodec::AVPacketFactory::create();
  int demuxError;
  int decodeError;
  auto frame = libffmpegxx::avutil::AVFrameFactory::create();
  do {
    demuxError = demuxer->read(packet);

    // Decode packet
    decodeError = decoders.at(packet->getStreamIndex())->decode(packet, frame);
    if (decodeError == EAGAIN) {
      std::cout << "Decoder for stream " << packet->getStreamIndex()
                << " not ready yet" << std::endl;
    } else if (decodeError != 0) {
      std::cout << "Decoder for stream " << packet->getStreamIndex()
                << " gave error: "
                << libffmpegxx::utils::Logger::avErrorToStr(decodeError)
                << std::endl;
      break;
    } else {
      std::cout << "Packet from stream " << packet->getStreamIndex()
                << " decoded successfully" << std::endl;
    }
  } while (demuxError >= 0);

  // You should flush the decoders if you want all the decoded content!
  packet->clear();
  for (auto &&[idx, decoder] : decoders) {
    int cnt{0};
    while (decoder->decode(packet, frame) == 0) {
      ++cnt;
    }
    std::cout << "Flushed " << cnt << " frames from decoder for stream " << idx
              << std::endl;
  }

  // Clean-up section
  delete packet;
  delete demuxer;
  for (auto &&[_, decoder] : decoders) {
    delete decoder;
  }
  delete frame;
}
