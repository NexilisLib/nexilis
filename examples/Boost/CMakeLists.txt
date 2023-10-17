cmake_minimum_required(VERSION 3.0)
project(impl_boost)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../../nexilis/ nexilis_dir)

find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})

add_compile_options(-Wall -Wextra -pedantic -Wconversion)
add_executable(impl_boost main.cc)

target_link_libraries(impl_boost nexilis ${Boost_LIBRARIES})

