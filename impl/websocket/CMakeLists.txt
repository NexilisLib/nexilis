cmake_minimum_required(VERSION 3.20)
project(server)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -static-libstdc++ -static-libgcc -pthread)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(websocketpp)
include_directories(src)
file(GLOB sources src/*.cc)
add_executable(server ${sources}) 
