cmake_minimum_required(VERSION 3.30)
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

add_compile_options(-Wall -Wextra -pedantic -static-libstdc++ -static-libgcc -pthread -std=c++17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++17")

set(SOURCES
    # Base
    src/nexilis/cmd_line_options.cc
    src/nexilis/protocol.cc
    src/nexilis/protocol_manager.cc
    src/nexilis/base_client.cc
    src/nexilis/base_room.cc
    src/nexilis/util.cc
    src/nexilis/room_command_type.cc
    src/nexilis/json.cc
    src/nexilis/room_data.cc

    # Movement
    src/nexilis/movement/movement.cc
    src/nexilis/movement/movement_2D.cc
    src/nexilis/movement/movement_3D.cc

    # Logger
    src/nexilis/logger/base_handler.cc
    src/nexilis/logger/console_handler.cc
    src/nexilis/logger/logger.cc
    src/nexilis/logger/log.cc
    src/nexilis/logger/loggable.cc

    # Server
    src/nexilis/server/server_protocol.cc
    src/nexilis/server/user.cc
    src/nexilis/server/command.cc
    src/nexilis/server/message_handler.cc
    src/nexilis/server/server_json.cc
    src/nexilis/server/room.cc
    src/nexilis/server/config.cc
    src/nexilis/server/settings.cc
    src/nexilis/server/client_storage.cc
    src/nexilis/server/room_storage.cc

    # Server protocols
    src/nexilis/server/protocol/nxboost/tcp_server.cc
    src/nexilis/server/protocol/af_unix/stream_server.cc

    # Client
    src/nexilis/client/client_api.cc
    src/nexilis/client/client_protocol.cc
    src/nexilis/client/packet.cc

    # Client protocols
    src/nexilis/client/protocol/nxboost/tcp_client.cc
    src/nexilis/client/protocol/af_unix/stream_client.cc

    #src/af_unix/sock_dgram/server.cc
    #src/af_unix/sock_dgram/client.cc

    #src/af_inet/udp_client.cc
    #src/af_inet/base_udp_server.cc
    #src/af_inet/udp_server.cc

    #src/af_inet/tcp_client.cc
    #src/af_inet/tcp_server.cc

    #src/boost/udp_client.cc
    #src/boost/udp_server.cc

    # NexilisC
    src/nexilisc/protocol_manager_c.cc

    src/nexilisc/logger/logger_c.cc
    src/nexilisc/logger/console_handler_c.cc
    src/nexilisc/logger/file_handler_c.cc

    src/nexilisc/server/settings_c.cc
)

add_library(nexilis ${SOURCES})
target_include_directories(nexilis PUBLIC include)
target_link_libraries(nexilis PRIVATE Boost::system Boost::json)
