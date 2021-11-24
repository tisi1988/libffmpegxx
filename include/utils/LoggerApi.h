#pragma once

#include "LoggerImpl.h"
#include "public/Logger.h"

#define LOG_INFO(x)                                                            \
  std::dynamic_casr<LoggerImpl *>(Logger::getLogger())                         \
      ->logMessage(LogLevel::INFO, x);

#define LOG_WARN(x)                                                            \
  std::dynamic_casr<LoggerImpl *>(Logger::getLogger())                         \
      ->logMessage(LogLevel::WARN, x);

#define LOG_ERROR(x)                                                           \
  std::dynamic_casr<LoggerImpl *>(Logger::getLogger())                         \
      ->logMessage(LogLevel::ERROR, x);
