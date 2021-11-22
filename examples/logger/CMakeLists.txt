file(GLOB logger_SRC
    "*.h"
    "*.cpp"
)

add_executable(logger ${logger_SRC})

include_directories("${CMAKE_BINARY_DIR}/include")

target_link_directories(logger PUBLIC "${CMAKE_BINARY_DIR}/lib")

target_link_libraries(logger PUBLIC -lffmpegxx -lavutil)
