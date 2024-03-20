cmake_minimum_required(VERSION 3.0)
project(nexilis)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# clang
#set(CMAKE_C_COMPILER "clang")
#set(CMAKE_CXX_COMPILER "clang++")

# Boost
find_package(Boost COMPONENTS system json REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# MySQL C API
find_library(MYSQL_CLIENT_LIBRARY mysqlclient)

add_compile_options(-Wall -Wextra -pedantic -static-libstdc++ -static-libgcc -pthread -std=c++17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(SOURCES
    src/logger/base_handler.cc
    src/logger/console_handler.cc

    src/authentication.cc
    src/client.cc
    src/command.cc
    src/config.cc
    src/vector3.cc
    src/ports.cc
    src/client_storage.cc
    src/client_protocol.cc
    src/server_protocol.cc
    src/client_api.cc
    src/log.cc
    src/loggable.cc
    src/message_handler.cc
    src/server_manager.cc
    src/packet.cc
    src/protocol.cc
    src/protocol_manager.cc
    src/json.cc
    src/room.cc
    src/room_storage.cc

    src/common/util.cc
    src/mysql/database.cc

    src/af_unix/sock_dgram/server.cc
    src/af_unix/sock_dgram/client.cc

    src/af_unix/sock_stream/server.cc
    src/af_unix/sock_stream/client.cc

    src/af_inet/udp_client.cc
    src/af_inet/base_udp_server.cc
    src/af_inet/udp_server.cc
    src/af_inet/tcp_client.cc
    src/af_inet/tcp_server.cc

    src/boost/tcp_client.cc
    src/boost/tcp_server.cc
    src/boost/udp_client.cc
    src/boost/udp_server.cc
)

add_library(nexilis ${SOURCES})

target_include_directories(nexilis PUBLIC include)
target_link_libraries(nexilis ${Boost_LIBRARIES} ${MYSQL_CLIENT_LIBRARY})
