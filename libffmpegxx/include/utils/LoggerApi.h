#pragma once

#include "../public/utils/Logger.h"
#include "LoggerImpl.h"

#define LOG_INFO(x)                                                            \
  dynamic_cast<utils::LoggerImpl *>(Logger::getLogger())                       \
      ->logMessage(utils::LogLevel::INFO, x);

#define LOG_WARN(x)                                                            \
  dynamic_cast<utils::LoggerImpl *>(utils::Logger::getLogger())                \
      ->logMessage(utils::LogLevel::WARN, x);

#define LOG_ERROR(x)                                                           \
  dynamic_cast<utils::LoggerImpl *>(Logger::getLogger())                       \
      ->logMessage(utils::LogLevel::ERROR, x);
