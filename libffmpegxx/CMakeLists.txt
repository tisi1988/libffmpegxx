# check for requirements
find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVFORMAT_LIBRARY avformat REQUIRED)

find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(AVUTIL_LIBRARY avutil REQUIRED)

# Gather source and header files
file(GLOB_RECURSE SOURCEFILES INC_ALL ${CMAKE_SOURCE_DIR}/libffmpegxx/*.cpp)
file(GLOB_RECURSE HEADERS INC_ALL ${CMAKE_SOURCE_DIR}/libffmpegxx/include/*.h)

# add the library target
add_library(ffmpegxxStatic STATIC ${SOURCEFILES} ${HEADERS})

# This would require also a set_target_properties(ffmpegxxShared ...)
#add_library(ffmpegxxShared SHARED version.cc)
include_directories("${CMAKE_SOURCE_DIR}/libffmpegxx/include")

configure_file(libffmpegxx_config.h.in "${CMAKE_SOURCE_DIR}/include/ffmpegxx_config.h")


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
