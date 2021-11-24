# check for requirements
find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVFORMAT_LIBRARY avformat REQUIRED)

find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(AVUTIL_LIBRARY avutil REQUIRED)

file(GLOB_RECURSE ffmpegxx_SRC "${CMAKE_SOURCE_DIR}/libffmpegxx/*" "${CMAKE_SOURCE_DIR}/include/*")

# add the executable
add_library(ffmpegxx STATIC ${ffmpegxx_SRC})

# This would require also a set_target_properties(ffmpegxxShared ...)
#add_library(ffmpegxxShared SHARED version.cc)

configure_file(libffmpegxx_config.h.in "${CMAKE_SOURCE_DIR}/include/ffmpegxx_config.h")

include_directories("${CMAKE_SOURCE_DIR}/include")

target_link_libraries(ffmpegxx PUBLIC -lavformat -lavutil)

set_target_properties(ffmpegxx
    PROPERTIES
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER include
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

add_custom_command(TARGET ffmpegxx PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       "${CMAKE_SOURCE_DIR}/include/public" "${CMAKE_BINARY_DIR}/include")
