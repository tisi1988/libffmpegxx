cmake_minimum_required(VERSION 3.10)

# set the project name
project(libffmpegxx VERSION 0.0.3)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add sources subdirectory
add_subdirectory(libffmpegxx)
add_subdirectory(examples)
