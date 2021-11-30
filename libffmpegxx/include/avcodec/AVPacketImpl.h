#pragma once

#include "../public/avcodec/IAVPacket.h"
#include "../public/time/Timebase.h"

struct AVPacket;

namespace libffmpegxx {
namespace time {
class Timestamp;
}; // namespace time

namespace avcodec {
class AVPacketImpl : public IAVPacket {
public:
  AVPacketImpl();
  AVPacketImpl(AVPacket *packet, time::Timebase const &tb,
               avformat::StreamInfo::Type type);
  explicit AVPacketImpl(const AVPacketImpl *const other);
  explicit AVPacketImpl(int size);
  ~AVPacketImpl();

  time::Timestamp getPts() const override;
  time::Timestamp getDts() const override;
  time::Timebase getTimebase() const override;
  int getSize() const override;
  int64_t getDuration() const override;
  const uint8_t *getRawData() const override;
  int getStreamIndex() const override;
  avformat::StreamInfo::Type getContentType() const override;
  int64_t getPosition() const override;
  int getFlags() const override;
  int getSideDataCount() const override;
  AVPacketSideData *getSideData() const override;
  void setTimestamp(time::Timestamp const &ts) override;
  void setTimestamp(time::Timestamp const &pts,
                    time::Timestamp const &dts) override;
  void setTimestamp(time::Seconds const &seconds) override;
  void setData(uint8_t *dataPtr, int size) override;
  void setContentType(avformat::StreamInfo::Type const &type) override;
  void setDuration(int duration) override;
  void setStreamIndex(int index) override;
  void setFlags(int flags) override;
  void setPosition(int64_t position) override;
  void clear() override;
  void refToPacket(const IAVPacket *const other) override;
  void moveToPacket(IAVPacket *const dst) override;

private:
  AVPacket *m_avPacket{nullptr};
  time::Timebase m_tb{0, 0};
  avformat::StreamInfo::Type m_type{avformat::StreamInfo::Type::NONE};
};
} // namespace avcodec
} // namespace libffmpegxx
