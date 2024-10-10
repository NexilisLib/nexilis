cmake_minimum_required(VERSION 3.13)
project(nexilis)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compile with clang.
#set(CMAKE_C_COMPILER "clang")
#set(CMAKE_CXX_COMPILER "clang++")

# Boost system, json. Asio is header only
find_package(Boost REQUIRED COMPONENTS system json)
include_directories(${Boost_INCLUDE_DIRS})

# MySQL C API
find_library(MYSQL_CLIENT_LIBRARY mysqlclient)

if(MYSQL_CLIENT_LIBRARY)
    add_definitions(-DHAS_MYSQL_CLIENT_LIBRARY)
    message("MySQL client library found")
else()
    message("MySQL client library not found")
endif()


add_compile_options(-Wall -Wextra -pedantic -static-libstdc++ -static-libgcc -pthread -std=c++17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++17")

set(SOURCES
    src/cmd_line_options.cc
    src/protocol.cc
    src/protocol_manager.cc
    src/base_client.cc
    src/util.cc
    src/command_type.cc
    src/json.cc

    # Server
    src/server/server_protocol.cc
    src/server/user.cc
    src/server/command.cc
    src/server/message_handler.cc
    src/server/server_json.cc
    src/server/room.cc
    src/server/config.cc
    src/server/settings.cc
    src/server/client_storage.cc
    src/server/room_storage.cc

    # Server protocols
    src/server/protocol/nxboost/tcp_server.cc
    src/server/protocol/af_unix/stream_server.cc

    # Client
    src/client/client_api.cc
    src/client/client_protocol.cc
    src/client/packet.cc

    # Client protocols
    src/client/protocol/nxboost/tcp_client.cc
    src/client/protocol/af_unix/stream_client.cc

    # Logger
    src/logger/base_handler.cc
    src/logger/console_handler.cc
    src/logger/logger.cc
    src/logger/log.cc
    src/logger/loggable.cc

    # Types
    src/types/vector3.cc

    # Object
    src/object/object2d.cc
    src/object/object3d.cc

    # MySQL
    src/mysql/database.cc

    #src/af_unix/sock_dgram/server.cc
    #src/af_unix/sock_dgram/client.cc

    #src/af_inet/udp_client.cc
    #src/af_inet/base_udp_server.cc
    #src/af_inet/udp_server.cc

    #src/af_inet/tcp_client.cc
    #src/af_inet/tcp_server.cc

    #src/boost/udp_client.cc
    #src/boost/udp_server.cc
)

add_library(nexilis ${SOURCES})
target_include_directories(nexilis PUBLIC include)

if(MYSQL_CLIENT_LIBRARY)
    target_link_libraries(nexilis PRIVATE Boost::system Boost::json ${MYSQL_CLIENT_LIBRARY})
else()
    target_link_libraries(nexilis PRIVATE Boost::system Boost::json)
endif()
