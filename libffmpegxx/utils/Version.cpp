#include "public/utils/Version.h"

#include "ffmpegxx_config.h"

#define STR_(x) #x
#define STR(x) STR_(x)

extern "C" {
#include <libavcodec/version.h>
#include <libavformat/version.h>
#include <libavutil/version.h>
}

// clang-format off
namespace libffmpegxx {
namespace utils {
std::string version() {
  return std::string(STR(libffmpegxx_VERSION_MAJOR)) + "." +
         STR(libffmpegxx_VERSION_MINOR) + "." +
         STR(libffmpegxx_VERSION_PATCH);
}

std::string buildInfo() {
  return "libffmpegxx:\t" + version() + "\n" +
         "libavcodec:\t\t" + STR(LIBAVCODEC_VERSION) + "\n"
         "libavformat:\t" + STR(LIBAVFORMAT_VERSION) + "\n" +
         "libavutil:\t\t" + STR(LIBAVUTIL_VERSION);
}
};  // namespace utils
};  // namespace libffmpegxx
