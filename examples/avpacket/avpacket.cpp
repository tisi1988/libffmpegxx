#include "avcodec/IAVPacket.h"

#include "time/Timestamp.h"

#include <iostream>

extern "C" {
#include <libavcodec/packet.h>
}

void printPacketData(libffmpegxx::avcodec::IAVPacket *p) {
  // clang-format off
  std::cout << "Packet data:"
            << "\n\tSize: " << p->getSize()
            << "\n\tStream idx: " << p->getStreamIndex()
            << "\n\tPTS: " << p->getPts().value()
            << "\n\tDTS: " << p->getDts().value()
            << "\n\tTimebase: " << p->getTimebase().toString()
            << "\n\tDuration: " << p->getDuration()
            << std::endl;
  // clang-format on
}

int main() {
  // Create an empty packet
  libffmpegxx::avcodec::IAVPacket *p =
      libffmpegxx::avcodec::AVPacketFactory::create();

  // Empty packet should not have anything
  printPacketData(p);

  // Let's create a raw AVPacket with some data
  AVPacket *raw = av_packet_alloc();
  raw->size = 80;
  raw->stream_index = 1;
  raw->pts = 2;
  raw->dts = 3;
  raw->duration = 3;

  // Then wrap it using libffmpegxx's one
  libffmpegxx::time::Timebase tb{1, 48'000};
  auto const type = libffmpegxx::avformat::StreamInfo::Type::VIDEO;
  libffmpegxx::avcodec::IAVPacket *p2 =
      libffmpegxx::avcodec::AVPacketFactory::create(raw, tb, type);

  // Data printed from this packet should match the above
  printPacketData(p2);
}
