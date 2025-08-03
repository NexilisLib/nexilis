cmake_minimum_required(VERSION 3.30)
project(unix_stream_server)

add_subdirectory(../../nexilis nexilis_dir)
add_executable(stream_server unix_stream_server.cc)
target_link_libraries(stream_server nexilis)
