cmake_minimum_required(VERSION 3.10)
project(net_swimmer)
include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SOURCES "src/*.cpp")

add_executable(net_swimmer ${SOURCES})

target_link_libraries(net_swimmer
    winhttp
)
