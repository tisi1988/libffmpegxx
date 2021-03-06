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
               avformat::StreamType type);
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
  avformat::StreamType getContentType() const override;
  int64_t getPosition() const override;
  int getFlags() const override;
  int getSideDataCount() const override;
  AVPacketSideData *getSideData() const override;
  void setTimebase(time::Timebase const &tb) override;
  void setTimestamp(time::Timestamp const &ts) override;
  void setTimestamp(time::Timestamp const &pts,
                    time::Timestamp const &dts) override;
  void setTimestamp(time::Seconds const &seconds) override;
  void setData(uint8_t *dataPtr, int size) override;
  void setContentType(avformat::StreamType const &type) override;
  void setDuration(int duration) override;
  void setStreamIndex(int index) override;
  void setFlags(int flags) override;
  void setPosition(int64_t position) override;
  void clear() override;
  void refToPacket(const IAVPacket *const other) override;
  void moveToPacket(IAVPacket *const dst) override;

  AVPacket *getWrappedPacket() const;

private:
  AVPacket *m_avPacket{nullptr};
  time::Timebase m_tb;
  avformat::StreamType m_type{avformat::StreamType::NONE};
};
} // namespace avcodec
} // namespace libffmpegxx
