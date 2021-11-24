#include <iostream>

#include "utils/time/Timestamp.h"
#include "utils/time/time_defs.h"

int main() {
  // Lets create an original timebase
  auto const tb1 = libffmpegxx::utils::time::Timebase(1, 48000);
  // And an original timestamp
  auto const ts1 = libffmpegxx::utils::time::Timestamp(1024, tb1);

  std::cout << "Time stamp value " << ts1.value() << " with time base "
            << tb1.toString() << ". That is " << ts1.toSeconds().count() << "s"
            << std::endl;

  // Lets create a target timebase
  auto const tb2 = libffmpegxx::utils::time::Timebase(1, 32000);
  std::cout << "Target timebase is " << tb2.toString() << std::endl;

  // Then move the original timestamp to the new timebase
  auto const ts2 = ts1.toTimebase(tb2);
  std::cout << "New time stamp value " << ts2.value() << " with time base "
            << tb2.toString() << ". That is " << ts2.toSeconds().count() << "s"
            << std::endl;
}
