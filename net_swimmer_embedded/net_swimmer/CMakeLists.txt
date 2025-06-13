cmake_minimum_required(VERSION 3.10)
project(net_swimmer)
include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SOURCES "src/*.cpp")

# --- Embedded JS engine (Duktape) ---
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party/duktape
)

add_executable(net_swimmer
    ${SOURCES}
    src/main.cpp
    src/tor_launcher.cpp
    src/mail_home.cpp
    src/util.cpp
    src/embedded_js.cpp
    third_party/duktape/duktape.c
)

target_link_libraries(net_swimmer
    winhttp
)
