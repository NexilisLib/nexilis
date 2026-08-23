cmake_minimum_required(VERSION 3.30)
project(boost_udp)

add_subdirectory(../../nexilis nexilis_dir)
add_executable(boost_udp main.cc)
target_link_libraries(boost_udp nexilis)
