#pragma once

#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif

extern "C" {
#include <libavutil/log.h>
}

#include <ostream>

namespace libffmpegxx {
namespace utils {
/**
 * @brief The LogLevel enum defines the available log levels.
 * These log levels map the FFMpeg's ones.
 */
enum class LogLevel { VERBOSE, DEBUG, INFO, WARN, ERROR, FATAL, QUIET };

/**
 * @brief The ILogger class defines the features of the logger.
 */
class ILogger {
public:
  /**
   * @brief Sets a new log level.
   * @param level The new log level. @see LogLevel.
   */
  virtual void setLogLevel(LogLevel const &level) = 0;

  /**
   * @brief Sets an output stream to print the log messages. By default,
   * the logger does not have any.
   * @param os The new output stream.
   * @note Every time an output stream is set, the library notifies
   * its version and the current log level.
   */
  virtual void setOutputStream(std::ostream *os) = 0;
};

/**
 * @brief The Logger class is the way to get access to the underlying
 * logging subsystem.
 */
class Logger {
public:
  /**
   * @return the Logger instance.
   */
  static ILogger *getLogger();

  /**
   * @brief Gets the error message associated to the given FFmpeg API error
   * code. This helper is needed since the original FFmpeg marco av_err2str()
   * does not work on C++. See
   * https://www.mail-archive.com/ffmpeg-user@ffmpeg.org/msg30270.html
   * @param error The FFmpeg API error code.
   * @return the error message mapped to the error code.
   */
  static std::string avErrorToStr(int error);

private:
  Logger() = default;
};
}; // namespace utils
}; // namespace libffmpegxx
