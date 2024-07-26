#include "utils/LoggerImpl.h"

#include "public/utils/Version.h"

#include "libavutil/error.h"

#include <mutex>
#include <sstream>
#include <string>

namespace libffmpegxx {
namespace utils {
static LoggerImpl g_logger;
static std::mutex g_ostreamMutex;

LogLevel av_log_levels_to_ffmpegxx_level(const int &loglevel) {
  switch (loglevel) {
  case AV_LOG_QUIET:
    return LogLevel::QUIET;
  case AV_LOG_PANIC:
    [[fallthrough]];
  case AV_LOG_FATAL:
    return LogLevel::FATAL;
  case AV_LOG_ERROR:
    return LogLevel::ERROR;
  case AV_LOG_WARNING:
    return LogLevel::WARN;
  case AV_LOG_INFO:
    return LogLevel::INFO;
  case AV_LOG_DEBUG:
    return LogLevel::DEBUG;
  case AV_LOG_TRACE:
    [[fallthrough]];
  case AV_LOG_VERBOSE:
    [[fallthrough]];
  default:
    return LogLevel::VERBOSE;
  }
}

std::stringstream g_ss;
char g_buffer[1024]{'\0'};

void log_cb(void *, int level, const char *szFmt, va_list varg) {

  {
    std::lock_guard<std::mutex> l(g_ostreamMutex);
    auto ostream = g_logger.getOutputStream();
    if (!ostream) {
      return;
    }
  }

  LogLevel const mapped_log_level = av_log_levels_to_ffmpegxx_level(level);

  // Discard if the level is not enabled
  if (mapped_log_level < g_logger.getLogLevel() ||
      mapped_log_level == libffmpegxx::utils::LogLevel::QUIET) {
    return;
  }

  int printPrefix = 1;
  const int msgLength = av_log_format_line2(
      nullptr, level, szFmt, varg, g_buffer, sizeof(g_buffer), &printPrefix);

  // FFmpeg can split a log line in several loggging callbacks
  // we need to check if the current line ends with an end line char
  // to log the accumulated string.
  if (g_buffer[msgLength - 1] == '\n') {
    // And some times the callback constains just the endl char
    if (msgLength > 1) {
      g_buffer[msgLength - 1] = '\0';
      g_ss << g_buffer;
    }
    g_logger.logMessage(mapped_log_level, g_ss.str());
    g_ss.str("");
    g_ss.clear();
  } else {
    // Current message does not contain an endl char, just append
    // its content for later.
    g_ss << g_buffer;
  }
}

LoggerImpl::LoggerImpl() {
  av_log_set_flags(AV_LOG_SKIP_REPEATED);
  av_log_set_callback(log_cb);
}

void LoggerImpl::setLogLevel(LogLevel const &level) { m_log_level = level; }

std::string logLevelToString(LogLevel level) {
  switch (level) {
  case LogLevel::FATAL:
    return "FATAL";
  case LogLevel::ERROR:
    return "ERROR";
  case LogLevel::WARN:
    return "WARN";
  case LogLevel::INFO:
    return "INFO";
  case LogLevel::DEBUG:
    return "DEBUG";
  case LogLevel::VERBOSE:
    return "VERBOSE";
  case LogLevel::QUIET:
  default:
    return "";
  }
}

void LoggerImpl::setOutputStream(std::ostream *os) {
  std::lock_guard<std::mutex> l(g_ostreamMutex);
  m_output_stream = os;
  if (m_output_stream) {
    auto const salute = logLevelToString(LogLevel::INFO) + " libffmpegxx v" +
                        libffmpegxx::utils::version() +
                        " Current log level is " +
                        logLevelToString(m_log_level);
    *m_output_stream << salute << std::endl;
  }
}

void LoggerImpl::logMessage(LogLevel const &level, std::string const &message) {
  std::lock_guard<std::mutex> l(g_ostreamMutex);
  if (!m_output_stream) {
    return;
  }

  auto const logLevelStr = logLevelToString(level);
  *m_output_stream << "[" << logLevelStr << "] " << message << std::endl;
}

LogLevel LoggerImpl::getLogLevel() const { return m_log_level; }

std::ostream *LoggerImpl::getOutputStream() const { return m_output_stream; }

ILogger *Logger::getLogger() { return &g_logger; }

std::string Logger::avErrorToStr(int error) {
  char buf[AV_ERROR_MAX_STRING_SIZE] = {0};
  return av_make_error_string(buf, AV_ERROR_MAX_STRING_SIZE, error);
}
}; // namespace utils
}; // namespace libffmpegxx
