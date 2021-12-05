#include "avcodec/AVPacketImpl.h"

#include "public/time/Timestamp.h"
#include "utils/LoggerApi.h"
#include "utils/exception.h"

#include <sstream>
#include <stdexcept>

extern "C" {
#if LIBAVCODEC_VERSION_MAJOR >= 58 && LIBAVCODEC_VERSION_MINOR >= 78 &&        \
    LIBAVCODEC_VERSION_MICRO >= 100
#include <libavcodec/packet.h>
#else
#include <libavcodec/avcodec.h>
#endif
#include <libavutil/avutil.h>
}

namespace libffmpegxx {
namespace avcodec {
IAVPacket *AVPacketFactory::create() { return new AVPacketImpl(); }

IAVPacket *AVPacketFactory::create(AVPacket *p, time::Timebase const &tb,
                                   avformat::StreamType type) {
  return new AVPacketImpl(p, tb, type);
}

IAVPacket *AVPacketFactory::create(const IAVPacket *const other) {
  auto const packetImpl = dynamic_cast<const AVPacketImpl *const>(other);

  if (!packetImpl) {
    LOG_FATAL("Could not handle packet while trying to reference it");
  }

  return new AVPacketImpl(packetImpl);
}

IAVPacket *AVPacketFactory::create(int size) {
  if (size < 0) {
    LOG_FATAL("Could not create packet with negative size");
  }

  return new AVPacketImpl(size);
}

AVPacketImpl::AVPacketImpl() {
  m_avPacket = av_packet_alloc();
  if (!m_avPacket) {
    LOG_FATAL("Could not allocate AVPacket")
  }
}

AVPacketImpl::AVPacketImpl(AVPacket *packet, time::Timebase const &tb,
                           avformat::StreamType type)
    : m_avPacket(packet), m_tb(tb), m_type(type) {
  if (!m_avPacket) {
    LOG_FATAL("AVPacket pointer cannot be null")
  }
}

AVPacketImpl::AVPacketImpl(const AVPacketImpl *const other) : AVPacketImpl() {
  if (!other) {
    LOG_FATAL("AVPacket pointer cannot be nullptr");
  }

  this->AVPacketImpl::refToPacket(other);
}

AVPacketImpl::AVPacketImpl(int size) {
  int const err = av_new_packet(m_avPacket, size);

  if (err != 0) {
    LOG_FATAL_FFMPEG_ERR(
        "Could not allocate AVPacket with size " + std::to_string(size), err);
  }
}

AVPacketImpl::~AVPacketImpl() {
  if (m_avPacket) {
    av_packet_free(&m_avPacket);
  }
}

time::Timestamp AVPacketImpl::getPts() const { return {m_avPacket->pts, m_tb}; }

time::Timestamp AVPacketImpl::getDts() const { return {m_avPacket->dts, m_tb}; }

time::Timebase AVPacketImpl::getTimebase() const { return m_tb; }

int AVPacketImpl::getSize() const { return m_avPacket->size; }

int64_t AVPacketImpl::getDuration() const { return m_avPacket->duration; }

const uint8_t *AVPacketImpl::getRawData() const {
  if (m_avPacket->buf->data) {
    return m_avPacket->buf->data;
  } else {
    return m_avPacket->data;
  }
}

int AVPacketImpl::getStreamIndex() const { return m_avPacket->stream_index; }

avformat::StreamType AVPacketImpl::getContentType() const { return m_type; }

int64_t AVPacketImpl::getPosition() const { return m_avPacket->pos; }

int AVPacketImpl::getFlags() const { return m_avPacket->flags; }

int AVPacketImpl::getSideDataCount() const {
  return m_avPacket->side_data_elems;
}

AVPacketSideData *AVPacketImpl::getSideData() const {
  return m_avPacket->side_data;
}

void AVPacketImpl::setTimebase(time::Timebase const &tb) { m_tb = tb; }

void AVPacketImpl::setTimestamp(time::Timestamp const &ts) {
  if (ts.getTimebase() != m_tb) {
    LOG_FATAL("Timestamp must match the packet's timebase! Passed: " +
              ts.getTimebase().toString() + " AVPacket: " + m_tb.toString());
  }

  m_avPacket->pts = m_avPacket->dts = ts.value();
}

void AVPacketImpl::setTimestamp(time::Timestamp const &pts,
                                time::Timestamp const &dts) {
  if (pts.getTimebase() != dts.getTimebase()) {
    LOG_FATAL("Both PTS and DTS must have the same timebase! PTS: " +
              pts.getTimebase().toString() +
              " DTS: " + pts.getTimebase().toString());
  }

  if (pts.getTimebase() != m_tb || dts.getTimebase() != m_tb) {
    LOG_FATAL("Both PTS and DTS must match the packet's timebase! PTS/DTS: " +
              pts.getTimebase().toString() + " AVPacket: " + m_tb.toString());
  }

  m_avPacket->pts = pts.value();
  m_avPacket->dts = dts.value();
}

void AVPacketImpl::setTimestamp(time::Seconds const &seconds) {
  if (m_avPacket->pts != m_avPacket->dts) {
    LOG_FATAL("Could not set timestamp since PTS and DTA do not match. " +
              std::to_string(m_avPacket->pts) +
              " != " + std::to_string(m_avPacket->dts));
  }

  auto tmp = time::Timestamp(m_avPacket->pts, m_tb);
  tmp.set(seconds);
  setTimestamp(tmp);
}

void AVPacketImpl::setData(uint8_t *dataPtr, int size) {
  if (size < 0) {
    LOG_FATAL("Data size cannot be negative");
  }

  if (size == 0 && dataPtr) {
    LOG_FATAL("Data size cannot be zero if the data is different from nullptr");
  }

  av_packet_unref(m_avPacket);
  int const err = av_packet_from_data(m_avPacket, dataPtr, size);
  if (err < 0) {
    LOG_FATAL_FFMPEG_ERR("Error setting the data to the AVPacket.", err);
  }
}

void AVPacketImpl::setContentType(avformat::StreamType const &type) {
  m_type = type;
}

void AVPacketImpl::setDuration(int duration) {
  if (duration <= 0) {
    LOG_FATAL("AVPacket duration must be a positive number");
  }

  m_avPacket->duration = duration;
}

void AVPacketImpl::setStreamIndex(int index) {
  if (index <= 0) {
    LOG_FATAL("AVPacket stream index must be a positive number");
  }

  m_avPacket->stream_index = index;
}

void AVPacketImpl::setFlags(int flags) { m_avPacket->flags = flags; }

void AVPacketImpl::setPosition(int64_t position) { m_avPacket->pos = position; }

void AVPacketImpl::clear() {
  av_packet_unref(m_avPacket);
  m_tb = time::Timebase();
  m_type = avformat::StreamType::NONE;
}

void AVPacketImpl::refToPacket(const IAVPacket *const other) {
  auto const packetImpl = dynamic_cast<const AVPacketImpl *const>(other);

  if (!packetImpl) {
    LOG_FATAL("Could not handle packet while trying to reference it");
  }

  this->AVPacketImpl::clear();

  int const err = av_packet_ref(m_avPacket, packetImpl->m_avPacket);
  if (err == 0) {
    m_tb = other->getTimebase();
    m_type = other->getContentType();
  } else {
    LOG_WARN("Could not reference packet. This one will be left blank")
    m_tb = time::Timebase();
    m_type = avformat::StreamType::NONE;
  }
}

void AVPacketImpl::moveToPacket(IAVPacket *const dst) {
  auto const packetImpl = dynamic_cast<const AVPacketImpl *const>(dst);

  if (!packetImpl) {
    LOG_FATAL("Could not handle packet while trying to move to it");
  }

  av_packet_move_ref(packetImpl->m_avPacket, m_avPacket);
  clear();
}

AVPacket *AVPacketImpl::getWrappedPacket() const { return m_avPacket; }
}; // namespace avcodec
}; // namespace libffmpegxx
