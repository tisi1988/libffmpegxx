#pragma once

#include "../time/time_defs.h"

struct AVPacket;
struct AVPacketSideData;

namespace libffmpegxx {
namespace time {
class Timebase;
class Timestamp;
}; // namespace time

namespace avcodec {
/**
 * @brief The IAVPacket class defines the AVPacket features.
 */
class IAVPacket {
public:
  /**
   * @brief The Type enum defines the possible data types.
   */
  enum class Type { NONE = 0, VIDEO, AUDIO, SUBTITLE, DATA };

  virtual ~IAVPacket() {}

  /**
   * @return the Presentation Timestamp. Timestamp (in Timebase units)
   * when this content should be played.
   */
  virtual time::Timestamp getPts() const = 0;

  /**
   * @return the Decoding Timestamp. Timestamp (in Timebase units)
   * when this content should be decoded.
   */
  virtual time::Timestamp getDts() const = 0;

  /**
   * @return the packet's timebase. Usually, this is the Timebase
   * this packet belongs to.
   */
  virtual time::Timebase getTimebase() const = 0;

  /**
   * @return packet's data size, in bytes.
   */
  virtual int getSize() const = 0;

  /**
   * @return duration of this packet in Timebase units, 0 if unknown.
   * Equals next_pts - this_pts in presentation order.
   */
  virtual int64_t getDuration() const = 0;

  /**
   * @return packet's raw data. Encoded video or audio.
   */
  virtual const uint8_t *getRawData() const = 0;

  /**
   * @return ID of the stream this packet belongs to.
   */
  virtual int getStreamIndex() const = 0;

  /**
   * @return the type
   */
  virtual Type getContentType() const = 0;

  /**
   * @return packet's position in the stream.
   */
  virtual int64_t getPosition() const = 0;

  /**
   * @return packet's flags.
   */
  virtual int getFlags() const = 0;

  /**
   * @return the amount of side data elements.
   */
  virtual int getSideDataCount() const = 0;

  /**
   * @return the side data.
   */
  virtual AVPacketSideData *getSideData() const = 0;

  /**
   * @brief Sets a new value for both PTS and DTS based the current Timebase.
   * @param ts New PTS/DTS timestamp.
   * @throws if the timebase of the given timestamp does not match the packet's
   * one.
   */
  virtual void setTimestamp(time::Timestamp const &ts) = 0;

  /**
   * @brief Sets a new values for PTS and DTS based on the current Timebase.
   * @param pts New PTS timestamp.
   * @param dts New DTS timestamp.
   * @throws if the timebase of the given timestamps does not match between
   * them or with the packet's one.
   */
  virtual void setTimestamp(time::Timestamp const &pts,
                            time::Timestamp const &dts) = 0;

  /**
   * @brief Tries to set both the PTS/DTS to match the given timestamp in
   * seconds. This will fail if the packet PTS and DTS do not match because we
   * cannot compute the decoding timestamp since we don't know the decoding
   * order.
   * @param seconds The new timestamp in seconds.
   * @throws if the timestamp is negative or if the internal PTS/DTS do not
   * match.
   */
  virtual void setTimestamp(time::Seconds const &seconds) = 0;

  /**
   * @brief Sets the data for the AVPacket. Any previous data is cleared out.
   * @param dataPtr The new data pointer.
   * @param size The size of the data in bytes.
   * @throws if size is negative or if size == 0 but dataPtr is not nullptr.
   */
  virtual void setData(uint8_t *dataPtr, int size) = 0;

  /**
   * @brief Sets the new content type of the packet.
   * @param type The new content type value.
   * @see Type
   */
  virtual void setContentType(Type const &type) = 0;

  /**
   * @brief Sets the packet duration.
   * @param duration The new packet duration (in timebase units).
   * @throws if duration is negative or zero.
   */
  virtual void setDuration(int duration) = 0;

  /**
   * @brief Sets the stream indez.
   * @param index The new stream index the packet belongs to.
   * @throws if stream index is negative.
   */
  virtual void setStreamIndex(int index) = 0;

  /**
   * @brief Set the flags of the packet.
   * @param flags The new flags value.
   */
  virtual void setFlags(int flags) = 0;

  /**
   * @brief Sets the new position of the packet in the stream.
   * @param position The new position value.
   */
  virtual void setPosition(int64_t position) = 0;

  /**
   * @brief Clear the packet data and resets it.
   */
  virtual void clear() = 0;

  /**
   * @brief Makes this packet reference to the same data another packet is
   * already referencing to.
   * @param other The packet whose data this packet will also reference.
   * @throws if the argument is nullptr
   */
  virtual void refToPacket(const IAVPacket *const other) = 0;

  /**
   * @brief Moves this packet to another packet. This packet will be cleared.
   * @param dst Destination packet
   * @throws if the argument is nullptr
   */
  virtual void moveToPacket(IAVPacket *const dst) = 0;
};

/**
 * @brief The AVPacketFactory class creates empty packets
 * or packets based on previously filled AVPackets.
 */
class AVPacketFactory {
public:
  /**
   * @brief Creates an empty packet with an empty Timebase.
   * @return the new packet.
   */
  static IAVPacket *create();

  /**
   * @brief Creates a new packet using a pre-existing AVPacket.
   * @param p The raw AVPacket to wrap.
   * @param tb The timebase the packet's timestamp units are based on (stream's
   * timebase usually).
   * @param type Type of content.
   * @return the new packet.
   */
  static IAVPacket *create(AVPacket *p, time::Timebase const &tb,
                           IAVPacket::Type type);

  /**
   * @brief Creates a new packet that references the same data as other packet.
   * @param other The packet to reference.
   * @return the new packet.
   */
  static IAVPacket *create(const IAVPacket *const other);

  /**
   * @brief Creates a new empty packet whose data buffers are already allocated.
   * @param size The size of the new packet's data buffers.
   * @return the new packet.
   */
  static IAVPacket *create(int size);
};
}; // namespace avcodec
}; // namespace libffmpegxx
