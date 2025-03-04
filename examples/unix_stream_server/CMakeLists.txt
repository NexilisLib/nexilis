cmake_minimum_required(VERSION 3.30)
project(unix_stream_server)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../../nexilis nexilis_dir)

add_executable(stream_server unix_stream_server.cc)
target_link_libraries(stream_server nexilis)