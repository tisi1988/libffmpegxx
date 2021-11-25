#pragma once

#include "../public/utils/Logger.h"
#include "LoggerImpl.h"

#define LOG_INFO(x)                                                            \
  std::dynamic_cast<LoggerImpl *>(Logger::getLogger())                         \
      ->logMessage(LogLevel::INFO, x);

#define LOG_WARN(x)                                                            \
  std::dynamic_cast<LoggerImpl *>(Logger::getLogger())                         \
      ->logMessage(LogLevel::WARN, x);

#define LOG_ERROR(x)                                                           \
  std::dynamic_cast<LoggerImpl *>(Logger::getLogger())                         \
      ->logMessage(LogLevel::ERROR, x);
