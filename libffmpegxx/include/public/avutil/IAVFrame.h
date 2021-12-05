#pragma once

#include "../avformat/MediaInfo.h"
#include "../time/time_defs.h"
#include "../utils/AVOptions.h"

struct AVFrame;

namespace libffmpegxx {
namespace time {
class Timebase;
class Timestamp;
}; // namespace time

namespace avutil {
/**
 * @brief The IAVFrame class defines the AVFrame features.
 */
class IAVFrame {
public:
  virtual ~IAVFrame() = default;

  /**
   * @return the Presentation Timestamp. Timestamp (in Timebase units)
   * when this content should be played.
   */
  virtual time::Timestamp getPts() const = 0;

  /**
   * @return the packet's timebase. Usually, this is the Timebase
   * this packet belongs to.
   */
  virtual time::Timebase getTimebase() const = 0;

  /**
   * @brief Sets a new timebase for the frame.
   * @param tb The new timebase.
   */
  virtual void setTimebase(time::Timebase const &tb) = 0;

  /**
   * @brief Sets a new value for PTS and DTS based on the current Timebase.
   * @param ts New PTS timestamp.
   * @throws if the timebase of the given timestamp does not match the packet's
   * one.
   */
  virtual void setTimestamp(time::Timestamp const &ts) = 0;

  /**
   * @brief Sets the PTS to match the given timestamp in
   * seconds.
   * @param seconds The new timestamp in seconds.
   * @throws if the timestamp is negative.
   */
  virtual void setTimestamp(time::Seconds const &seconds) = 0;

  /**
   * @brief Clear the frame data and resets it.
   */
  virtual void clear() = 0;

  /**
   * @brief References the current frame to the given one.
   * @param other The frame to reference to.
   */
  virtual void refToFrame(const IAVFrame *const other) = 0;

  /**
   * @return pointer to the picture/channel planes.
   */
  virtual std::vector<uint8_t *> getData() = 0;

  /**
   * @return For video, size in bytes of each picture line. For audio, size in
   * bytes of each plane. For audio, only linesize[0] may be set. For planar
   * audio, each channel plane must be the same size.
   */
  virtual std::vector<int> getLineSizes() const = 0;

  /**
   * @return number of audio samples (per channel)
   */
  virtual int getSampleCount() const = 0;

  /**
   * @return AVPixelFormat or AVSampleFormat.
   */
  virtual int getFormat() const = 0;

  /**
   * @return true if the frame is a video keyframe. False otherwise.
   */
  virtual bool isKeyframe() const = 0;

  /**
   * @return type of video image.
   */
  virtual AVPictureType getPictureType() const = 0;

  /**
   * @return sample aspect ratio for the video frame, 0/1 if
   * unknown/unspecified.
   */
  virtual AVRational getSampleAspectRatio() const = 0;

  /**
   * @return When decoding, this signals how much the picture must be delayed.
   * extra_delay = repeat_pict / (2*fps)
   */
  virtual int getRepeatPicture() const = 0;

  /**
   * @return The content of the picture is interlaced.
   */
  virtual bool isInterlaced() const = 0;

  /**
   * @return If the content is interlaced, is top field displayed first.
   */
  virtual bool isTopFieldFirst() const = 0;

  /**
   * @return Sample rate of the audio data.
   */
  virtual int getAudioSamplerate() const = 0;

  /**
   * @return number of audio channels, only used for audio.
   */
  virtual int getAudioChannels() const = 0;

  /**
   * @return Channel layout of the audio data.
   */
  virtual uint64_t getAudioChannelLayout() const = 0;

  /**
   * @return frame side data.
   */
  virtual std::vector<AVFrameSideData *> getSideData() = 0;

  /**
   * @return Frame flags, a combination of AV_FRAME_FLAGS.
   */
  virtual int getFlags() const = 0;

  /**
   * @return MPEG vs JPEG YUV range.
   */
  virtual AVColorRange getColorRange() const = 0;

  /**
   * @return color primaries.
   */
  virtual AVColorPrimaries getColorPrimaries() const = 0;

  /**
   * @return color transfer characteristics.
   */
  virtual AVColorTransferCharacteristic
  getColorTransferCharacteristics() const = 0;

  /**
   * @return color space.
   */
  virtual AVColorSpace getColorSpace() const = 0;

  /**
   * @return chroma location.
   */
  virtual AVChromaLocation getChromaLocation() const = 0;

  /**
   * @return frame metadata.
   */
  virtual utils::AVOptions getMetadata() const = 0;

  /**
   * @return decode error flags of the frame, set to a combination of
   * FF_DECODE_ERROR_xxx flags if the decoder produced a frame, but there were
   * errors during the decoding.
   */
  virtual int getDecodingErrorFlags() const = 0;
};

/**
 * @brief The AVFrameFactory class creates empty frames
 * or frames based on previously filled AVFrames.
 */
class AVFrameFactory {
public:
  /**
   * @brief Creates an empty frame.
   * @return the new frame.
   */
  static IAVFrame *create();

  /**
   * @brief Creates a new frame using a pre-existing AVFrame.
   * @param f The raw AVFrame to wrap.
   * @param tb The timebase the timestamp of the frame is based on.
   * @return the new frame.
   */
  static IAVFrame *create(AVFrame *f, time::Timebase const &tb);

  /**
   * @brief Creates a new frame that references the same data as other frame.
   * @param other The frame to reference.
   * @return the new frame.
   */
  static IAVFrame *create(const IAVFrame *const other);
};
}; // namespace avutil
}; // namespace libffmpegxx
