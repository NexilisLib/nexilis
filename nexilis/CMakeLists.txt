cmake_minimum_required(VERSION 3.0)
project(nexilis)

include(../external/websocketpp-build/websocketppConfig.cmake)

configure_websocketpp()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -static-libstdc++ -static-libgcc -pthread)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(SOURCES src/command.cc src/vector3.cc)

add_library(nexilis ${SOURCES})

#add_subdirectory(../websocketpp/ websocketppDir)

#target_include_directories(nexilis PUBLIC include PRIVATE ../websocketpp/)

target_link_libraries(nexilis PUBLIC include PRIVATE ${websocketpp_target})
