function(createTestApp appName libraries)
    file(GLOB sources
        "*.h"
        "*.cpp"
    )

    add_executable(${appName} ${sources})

    include_directories("${CMAKE_BINARY_DIR}/include")

    target_link_directories(${appName} PUBLIC "${CMAKE_BINARY_DIR}/lib")

    target_link_libraries(${appName} PUBLIC -lffmpegxxStatic ${libraries})

    add_dependencies(${appName} ffmpegxxStatic)
endfunction()

# Add subdirectory for each example application
add_subdirectory(version)
add_subdirectory(logger)
add_subdirectory(timestamp)
add_subdirectory(avformat)
add_subdirectory(avcodec)
