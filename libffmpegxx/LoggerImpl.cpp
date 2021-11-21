#include "LoggerImpl.h"

#include <string>

namespace libffmpegxx {
namespace utils {
static LoggerImpl g_logger;

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

void log_cb(void *, int level, const char *szFmt, va_list varg) {

  auto ostream = g_logger.getOutputStream();
  if (!ostream) {
    return;
  }

  LogLevel const mapped_log_level = av_log_levels_to_ffmpegxx_level(level);

  // Discard if the level is not enabled
  if (mapped_log_level < g_logger.getLogLevel() ||
      mapped_log_level == libffmpegxx::utils::LogLevel::QUIET) {
    return;
  }

  char buffer[1024]{};

  int printPrefix = 1;
  av_log_format_line(nullptr, level, szFmt, varg, buffer, sizeof(buffer),
                     &printPrefix);

  *ostream << buffer;
}

LoggerImpl::LoggerImpl() {
  av_log_set_flags(AV_LOG_SKIP_REPEATED);
  av_log_set_callback(log_cb);
}

void LoggerImpl::setLogLevel(LogLevel const &level) { m_log_level = level; }

void LoggerImpl::setOutputStream(std::ostream *os) { m_output_stream = os; }

LogLevel LoggerImpl::getLogLevel() const { return m_log_level; }

std::ostream *LoggerImpl::getOutputStream() const { return m_output_stream; }

ILogger *Logger::getLogger() { return &g_logger; }
}; // namespace utils
}; // namespace libffmpegxx
