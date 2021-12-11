#include "avcodec/IAVPacket.h"
#include "avcodec/IDecoder.h"
#include "avcodec/IEncoder.h"
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

  // Create encoders (using same codecs as the original media)
  // Here you should create the StreamInfo instances for any
  // other codecs for encoding
  std::map<int, libffmpegxx::avcodec::IEncoder *> encoders;
  for (auto &&[idx, info] : info.streamsInfo) {
    encoders.insert({idx, libffmpegxx::avcodec::EncoderFactory::create(info)});
  }

  // Start reading packets
  libffmpegxx::avcodec::IAVPacket *demuxingPacket =
      libffmpegxx::avcodec::AVPacketFactory::create();
  libffmpegxx::avcodec::IAVPacket *muxingPacket =
      libffmpegxx::avcodec::AVPacketFactory::create();
  int demuxError;
  int decodeError;
  int encodeError;
  auto frame = libffmpegxx::avutil::AVFrameFactory::create();
  do {
    demuxError = demuxer->read(demuxingPacket);

    int const streamIdx = demuxingPacket->getStreamIndex();

    // Decode packet
    decodeError = decoders.at(streamIdx)->decode(demuxingPacket, frame);
    if (decodeError == AVERROR(EAGAIN)) {
      std::cout << "Decoder for stream " << streamIdx << " not ready yet"
                << std::endl;
      continue;
    } else if (decodeError != 0) {
      std::cout << "Decoder for stream " << streamIdx << " gave error: "
                << libffmpegxx::utils::Logger::avErrorToStr(decodeError)
                << std::endl;
      break;
    }

    // Encode the frame
    encodeError = encoders.at(streamIdx)->encode(frame, muxingPacket);
    if (encodeError == AVERROR(EAGAIN)) {
      std::cout << "Encoder for stream " << streamIdx << " not ready yet"
                << std::endl;
      continue;
    } else if (encodeError != 0) {
      std::cout << "Encoder for stream " << streamIdx << " gave error: "
                << libffmpegxx::utils::Logger::avErrorToStr(encodeError)
                << std::endl;
      break;
    }
  } while (demuxError >= 0);

  // You should flush the decoders if you want all the decoded content!
  for (auto &&[idx, decoder] : decoders) {
    std::vector<libffmpegxx::avutil::IAVFrame *> frames;
    decoder->flush(frames);
    std::cout << "Flushed " << frames.size()
              << " frames from decoder for stream " << idx << std::endl;
  }

  // You should also flush the encoders!
  for (auto &&[idx, encoder] : encoders) {
    std::vector<libffmpegxx::avcodec::IAVPacket *> packets;
    encoder->flush(packets);
    std::cout << "Flushed " << packets.size()
              << " packets from encoder for stream " << idx << std::endl;
    for (auto &&p : packets) {
      delete p;
    }
  }

  // Clean-up section
  delete demuxingPacket;
  delete muxingPacket;
  delete demuxer;
  for (auto &&[_, decoder] : decoders) {
    delete decoder;
  }
  for (auto &&[_, encoder] : encoders) {
    delete encoder;
  }
  delete frame;
}
