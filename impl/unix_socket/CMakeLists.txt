cmake_minimum_required(VERSION 3.20)
project(impl_unix_socket)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../../nexilis/ nexilis_dir)

add_compile_options(-Wall -Wextra -pedantic -Wconversion)

include_directories(src)
file(GLOB sources src/*.cc)

add_executable(impl_unix_socket ${sources})
target_link_libraries(impl_unix_socket nexilis)
