#include "Logger.h"
#include "Version.h"

#include <iostream>

int main() {
  auto logger = libffmpegxx::utils::Logger::getLogger();

  logger->setLogLevel(libffmpegxx::utils::LogLevel::VERBOSE);

  // When the output stream is registered, the library notifies its version
  // so we should see it on the console
  std::ostream *oStream = &std::cout;
  logger->setOutputStream(oStream);
}
