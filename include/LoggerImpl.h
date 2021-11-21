#pragma once

#include "public/Logger.h"

namespace libffmpegxx {
namespace utils {
class LoggerImpl : public ILogger {
public:
  LoggerImpl();

  void setLogLevel(LogLevel const &level) override;
  void setOutputStream(std::ostream *os) override;

  LogLevel getLogLevel() const;
  std::ostream *getOutputStream() const;

private:
  void avlog_cb(void *, int level, const char *szFmt, va_list varg);

  LogLevel m_log_level{LogLevel::QUIET};
  std::ostream *m_output_stream{nullptr};
};
}; // namespace utils
}; // namespace libffmpegxx
