#pragma once

#include "../public/avutil/IAVFrame.h"

namespace libffmpegxx {
namespace avutil {
class AVFrameImpl : public IAVFrame {
public:
  AVFrameImpl();
  AVFrameImpl(AVFrame *frame, time::Timebase const &tb);
  explicit AVFrameImpl(const AVFrameImpl *const other);
  ~AVFrameImpl();

  time::Timestamp getPts() const override;
  time::Timebase getTimebase() const override;
  void setTimebase(time::Timebase const &tb) override;
  void setTimestamp(time::Timestamp const &ts) override;
  void setTimestamp(time::Seconds const &seconds) override;
  void clear() override;
  void refToFrame(const IAVFrame *const other) override;

  std::vector<uint8_t *> getData() override;
  std::vector<int> getLineSizes() const override;
  int getSampleCount() const override;
  int getFormat() const override;
  bool isKeyframe() const override;
  AVPictureType getPictureType() const override;
  AVRational getSampleAspectRatio() const override;
  int getRepeatPicture() const override;
  bool isInterlaced() const override;
  bool isTopFieldFirst() const override;
  int getAudioSamplerate() const override;
  int getAudioChannels() const override;
  uint64_t getAudioChannelLayout() const override;
  std::vector<AVFrameSideData *> getSideData() override;
  int getFlags() const override;
  AVColorRange getColorRange() const override;
  AVColorPrimaries getColorPrimaries() const override;
  AVColorTransferCharacteristic
  getColorTransferCharacteristics() const override;
  AVColorSpace getColorSpace() const override;
  AVChromaLocation getChromaLocation() const override;
  utils::AVOptions getMetadata() const override;
  int getDecodingErrorFlags() const override;

private:
  time::Timebase m_tb;
  AVFrame *m_avframe{nullptr};
};
}; // namespace avutil
}; // namespace libffmpegxx
