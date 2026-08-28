cmake_minimum_required(VERSION 3.25)
project(skip_auth)

add_subdirectory(../../nexilis nexilis_dir)
add_executable(skip_auth main.cc)
target_link_libraries(skip_auth nexilis)
