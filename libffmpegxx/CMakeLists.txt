function(configureLibTarget TARGET_NAME)
    target_link_libraries(${TARGET_NAME} PUBLIC -lavformat -lavutil)

    set_target_properties(${TARGET_NAME}
        PROPERTIES
        VERSION ${PROJECT_VERSION}
        PUBLIC_HEADER "${CMAKE_SOURCE_DIR}/libffmpegxx/include/public"
        PRIVATE_HEADER "${CMAKE_SOURCE_DIR}/libffmpegxx/include"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        OUTPUT_NAME ${PROJECT_NAME}
        )

    add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_directory
                           "${CMAKE_SOURCE_DIR}/libffmpegxx/include/public" "${CMAKE_BINARY_DIR}/include")
endfunction()

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
add_library(ffmpegxxShared SHARED ${SOURCEFILES} ${HEADERS})

include_directories("${CMAKE_SOURCE_DIR}/libffmpegxx/include")

configure_file(libffmpegxx_config.h.in "${CMAKE_SOURCE_DIR}/include/ffmpegxx_config.h")

configureLibTarget(ffmpegxxStatic)
configureLibTarget(ffmpegxxShared)
