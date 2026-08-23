cmake_minimum_required(VERSION 3.25)
project(boost)

add_subdirectory(../../nexilis nexilis_dir)
add_executable(boost main.cc)
target_link_libraries(boost nexilis)
