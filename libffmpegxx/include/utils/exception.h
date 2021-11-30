#pragma once

#define THROW_FFMPEG_ERR_DESCRIPTION(description, errCode)                     \
  std::stringstream sstream;                                                   \
  sstream << description;                                                      \
  sstream << "Error code " << std::to_string(errCode) << ": ";                 \
  sstream << utils::Logger::avErrorToStr(errCode);                             \
  throw std::runtime_error(sstream.str());
  
