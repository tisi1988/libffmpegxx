file(GLOB version_SRC
    "*.h"
    "*.cpp"
)

add_executable(version ${version_SRC})

include_directories("${CMAKE_BINARY_DIR}/include")

target_link_directories(version PUBLIC "${CMAKE_BINARY_DIR}/lib")

target_link_libraries(version PUBLIC -lffmpegxx)
