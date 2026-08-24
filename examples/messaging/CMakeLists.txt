cmake_minimum_required(VERSION 3.25)
project(messaging)

add_subdirectory(../../nexilis nexilis_dir)
add_executable(messaging main.cc)
target_link_libraries(messaging nexilis)
