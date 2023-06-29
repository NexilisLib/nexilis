cmake_minimum_required(VERSION 3.0)
project(nexilis)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -static-libstdc++ -static-libgcc -pthread)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(SOURCES src/command.cc src/vector3.cc src/context.cc)

# Create the calc library
add_library(nexilis ${SOURCES})

# Specify the include directories for calc
target_include_directories(nexilis PUBLIC include)
