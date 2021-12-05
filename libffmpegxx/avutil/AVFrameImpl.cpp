#include "avutil/AVFrameImpl.h"

#include "public/time/Timebase.h"
#include "public/time/Timestamp.h"

#include "utils/LoggerApi.h"

namespace libffmpegxx {
namespace utils {
extern AVOptions fromAVDictionary(AVDictionary *dict);
}

namespace avutil {
IAVFrame *AVFrameFactory::create() { return new AVFrameImpl(); }

IAVFrame *AVFrameFactory::create(AVFrame *f, time::Timebase const &tb) {
  return new AVFrameImpl(f, tb);
}

IAVFrame *AVFrameFactory::create(const IAVFrame *const other) {
  auto const frameImpl = dynamic_cast<const AVFrameImpl *const>(other);

  if (!frameImpl) {
    LOG_FATAL("Could not handle frame while trying to reference it");
  }

  return new AVFrameImpl(frameImpl);
}

AVFrameImpl::AVFrameImpl() {
  m_avframe = av_frame_alloc();
  if (!m_avframe) {
    LOG_FATAL("Could not allocate frame");
  }
}

AVFrameImpl::AVFrameImpl(AVFrame *frame, time::Timebase const &tb)
    : m_avframe(frame), m_tb(tb) {
  if (!m_avframe) {
    LOG_FATAL("AVPacket pointer cannot be null");
  }
}

AVFrameImpl::AVFrameImpl(const AVFrameImpl *const other) : AVFrameImpl() {
  if (!other) {
    LOG_FATAL("AVFrame pointer cannot be nullptr");
  }

  this->AVFrameImpl::refToFrame(other);
}

AVFrameImpl::~AVFrameImpl() {
  if (m_avframe) {
    av_frame_free(&m_avframe);
  }
}

time::Timestamp AVFrameImpl::getPts() const { return {m_avframe->pts, m_tb}; }

time::Timebase AVFrameImpl::getTimebase() const { return m_tb; }

void AVFrameImpl::setTimebase(time::Timebase const &tb) { m_tb = tb; }

void AVFrameImpl::setTimestamp(time::Timestamp const &ts) {
  if (ts.getTimebase() != m_tb) {
    throw std::runtime_error(
        "Timestamp must match the packet's timebase! Passed: " +
        ts.getTimebase().toString() + " AVPacket: " + m_tb.toString());
  }

  m_avframe->pts = ts.value();
}

void AVFrameImpl::setTimestamp(time::Seconds const &seconds) {
  auto tmp = time::Timestamp(m_avframe->pts, m_tb);
  tmp.set(seconds);
  setTimestamp(tmp);
}

void AVFrameImpl::clear() {
  av_frame_unref(m_avframe);
  m_tb = time::Timebase();
}

void AVFrameImpl::refToFrame(const IAVFrame *const other) {
  auto const frameImpl = dynamic_cast<const AVFrameImpl *const>(other);

  if (!frameImpl) {
    throw std::runtime_error(
        "Could not handle packet while trying to reference it");
  }

  this->AVFrameImpl::clear();

  int const err = av_frame_ref(m_avframe, frameImpl->m_avframe);
  if (err == 0) {
    m_tb = other->getTimebase();
  } else {
    LOG_WARN("Could not reference frame. This one will be left blank")
    m_tb = time::Timebase();
  }
}

std::vector<uint8_t *> AVFrameImpl::getData() {
  return std::vector<uint8_t *>{m_avframe->data,
                                m_avframe->data + AV_NUM_DATA_POINTERS};
}

std::vector<int> AVFrameImpl::getLineSizes() const {
  return std::vector<int>{m_avframe->linesize,
                          m_avframe->linesize + AV_NUM_DATA_POINTERS};
}

int AVFrameImpl::getSampleCount() const { return m_avframe->nb_samples; }

int AVFrameImpl::getFormat() const { return m_avframe->format; }

bool AVFrameImpl::isKeyframe() const { return m_avframe->key_frame == 1; }

AVPictureType AVFrameImpl::getPictureType() const {
  return m_avframe->pict_type;
}

AVRational AVFrameImpl::getSampleAspectRatio() const {
  return m_avframe->sample_aspect_ratio;
}

int AVFrameImpl::getRepeatPicture() const { return m_avframe->repeat_pict; }

bool AVFrameImpl::isInterlaced() const {
  return m_avframe->interlaced_frame == 1;
}

bool AVFrameImpl::isTopFieldFirst() const {
  return m_avframe->top_field_first == 1;
}

int AVFrameImpl::getAudioSamplerate() const { return m_avframe->sample_rate; }

int AVFrameImpl::getAudioChannels() const { return m_avframe->nb_samples; }

uint64_t AVFrameImpl::getAudioChannelLayout() const {
  return m_avframe->channel_layout;
}

std::vector<AVFrameSideData *> AVFrameImpl::getSideData() {
  return std::vector<AVFrameSideData *>{
      m_avframe->side_data, m_avframe->side_data + m_avframe->nb_side_data};
}

int AVFrameImpl::getFlags() const { return m_avframe->flags; }

AVColorRange AVFrameImpl::getColorRange() const {
  return m_avframe->color_range;
}

AVColorPrimaries AVFrameImpl::getColorPrimaries() const {
  return m_avframe->color_primaries;
}

AVColorTransferCharacteristic
AVFrameImpl::getColorTransferCharacteristics() const {
  return m_avframe->color_trc;
}

AVColorSpace AVFrameImpl::getColorSpace() const {
  return m_avframe->colorspace;
}

AVChromaLocation AVFrameImpl::getChromaLocation() const {
  return m_avframe->chroma_location;
}

utils::AVOptions AVFrameImpl::getMetadata() const {
  return utils::fromAVDictionary(m_avframe->metadata);
}

int AVFrameImpl::getDecodingErrorFlags() const {
  return m_avframe->decode_error_flags;
}

}; // namespace avutil
}; // namespace libffmpegxx
