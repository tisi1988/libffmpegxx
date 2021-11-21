#include "version.h"

#include <iostream>

int main(int argc, char** argv) {
  std::cout << libffmpegxx::utils::buildInfo() << std::endl;
}
