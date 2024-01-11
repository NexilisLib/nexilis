cmake_minimum_required(VERSION 3.0)
project(nexilis)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -static-libstdc++ -static-libgcc -pthread)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(SOURCES
    src/command.cc
    src/vector3.cc
    src/ports.cc
    src/client_storage.cc
    src/boost/boost_io_context.cc
    src/log.cc
    src/client.cc

    src/af_unix/unix_socket_server.cc
    src/af_unix/unix_socket_sender.cc

    src/af_inet/base_udp_server.cc
    src/af_inet/udp_sender.cc
    src/af_inet/udp_server.cc

	src/websocket/websocket.cc)

add_library(nexilis ${SOURCES})

target_include_directories(nexilis PUBLIC include ../external/websocketpp)
