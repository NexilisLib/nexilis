cmake_minimum_required(VERSION 3.20)

project(sock_stream_client)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../../../nexilis/ nexilis_dir)

add_compile_options(-Wall -Wextra -pedantic -Wconversion)

include_directories(src)
FILE(GLOB sources src/*.cc)

add_executable(sock_stream_client ${sources})

target_link_libraries(sock_stream_client nexilis)

