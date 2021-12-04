#pragma once

#include "utils/LoggerApi.h"

#define LOG_FATAL_FFMPEG_ERR(description, errCode)                     \
  std::stringstream sstream;                                                   \
  sstream << description;                                                      \
  sstream << "Error code " << std::to_string(errCode) << ": ";                 \
  sstream << utils::Logger::avErrorToStr(errCode);                             \
  LOG_FATAL(sstream.str());
