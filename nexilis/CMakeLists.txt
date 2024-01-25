cmake_minimum_required(VERSION 3.0)
project(nexilis)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Boost
find_package(Boost COMPONENTS system REQUIRED)    
include_directories(${Boost_INCLUDE_DIRS})

add_compile_options(-Wall -Wextra -pedantic -static-libstdc++ -static-libgcc -pthread)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(SOURCES
    src/command.cc
    src/vector3.cc
    src/ports.cc
    src/client_storage.cc
    src/log.cc
    src/client.cc
    src/message_handler.cc
    src/server_manager.cc

    src/af_unix/sock_dgram/unix_socket_server.cc
    src/af_unix/sock_dgram/unix_socket_sender.cc
    src/af_unix/sock_dgram/unix_socket_client.cc

    src/af_unix/sock_stream/server.cc
    src/af_unix/sock_stream/client.cc

    src/af_inet/udp_client.cc
    src/af_inet/udp_server.cc
    src/af_inet/base_udp_server.cc
    src/af_inet/tcp_client.cc
    src/af_inet/tcp_server.cc

    src/boost/tcp_client.cc
    src/boost/tcp_server.cc

    src/client_api/client_api.cc
    src/websocket/websocket.cc

    src/common/af_inet_udp_sender.cc)

add_library(nexilis ${SOURCES})

target_include_directories(nexilis PUBLIC include ../external/websocketpp)
target_link_libraries(nexilis ${Boost_LIBRARIES})