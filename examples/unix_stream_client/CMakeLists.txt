cmake_minimum_required(VERSION 3.30)
project(stream_client)

add_subdirectory(../../nexilis nexilis_dir)
add_executable(stream_client unix_stream_client.cc)
target_link_libraries(stream_client nexilis)
