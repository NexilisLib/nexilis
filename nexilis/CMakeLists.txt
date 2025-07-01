cmake_minimum_required(VERSION 3.25)
project(nexilis)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(BUILD_SHARED_LIBS ON)

# Compile with clang.
#set(CMAKE_C_COMPILER "clang")
#set(CMAKE_CXX_COMPILER "clang++")

add_compile_options(-Wall -Wextra -pedantic -static-libstdc++ -static-libgcc -pthread -std=c++17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++17")

set(SOURCES
    # Base
    src/nexilis/nx_class.cc
    src/nexilis/cmd_line_options.cc
    src/nexilis/protocol.cc
    src/nexilis/protocol_manager.cc
    src/nexilis/base_client.cc
    src/nexilis/base_room.cc
    src/nexilis/util.cc
    src/nexilis/room_command_type.cc
    src/nexilis/json.cc
    src/nexilis/room_data.cc
    src/nexilis/command_type.cc
    src/nexilis/ports.cc

    src/nexilis/object/object_2d.cc
    src/nexilis/object/object_3d.cc

    # Movement
    src/nexilis/movement/movement_data.cc
    src/nexilis/movement/movement_2D.cc
    src/nexilis/movement/movement_3D.cc

    # Logger
    src/nexilis/logger/base_handler.cc
    src/nexilis/logger/console_handler.cc
    src/nexilis/logger/logger.cc
    src/nexilis/logger/log.cc
    src/nexilis/logger/file_log.cc

    # Server
    src/nexilis/server/message/base_message.cc
    src/nexilis/server/message/message.cc
    src/nexilis/server/message/auth_message.cc
    src/nexilis/server/message/error_message.cc
    src/nexilis/server/message/message_handler.cc

    src/nexilis/server/server_protocol.cc
    src/nexilis/server/user.cc
    src/nexilis/server/command.cc
    src/nexilis/server/server_json.cc
    src/nexilis/server/room.cc
    src/nexilis/server/config.cc
    src/nexilis/server/settings.cc
    src/nexilis/server/client_storage.cc
    src/nexilis/server/room_storage.cc


    # Server protocols
    src/nexilis/server/protocol/nxboost/tcp_server.cc
    src/nexilis/server/protocol/nxboost/udp_server.cc
    src/nexilis/server/protocol/af_unix/stream_server.cc

    # Client
    src/nexilis/client/server_data.cc
    src/nexilis/client/client_api.cc
    src/nexilis/client/client_protocol.cc
    src/nexilis/client/client_session.cc
    src/nexilis/client/packet.cc
    src/nexilis/client/room.cc

    # Client protocols
    src/nexilis/client/protocol/nxboost/tcp_client.cc
    src/nexilis/client/protocol/nxboost/udp_client.cc
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
    src/nexilisc/nexilis_promise.cc
    src/nexilisc/nexilis_waiter.cc
    src/nexilisc/protocol_manager_c.cc
    src/nexilisc/nx_data_c.cc
    src/nexilisc/types/vector3_c.cc

    src/nexilisc/logger/logger_c.cc
    src/nexilisc/logger/console_handler_c.cc
    src/nexilisc/logger/file_handler_c.cc
    src/nexilisc/logger/function_handler_c.cc
    src/nexilisc/logger/log_c.cc

    src/nexilisc/server/settings_c.cc

    src/nexilisc/client/client_session_c.cc
    src/nexilisc/client/client_api_c.cc
    src/nexilisc/client/server_data_c.cc
    src/nexilisc/client/packet_c.cc
    src/nexilisc/client/room_c.cc
    src/nexilisc/client/rooms_collection.cc

    src/nexilisc/server/protocol/unix_stream_server_c.cc
    src/nexilisc/server/protocol/boost_tcp_server_c.cc
    src/nexilisc/client/protocol/boost_tcp_client_c.cc
)

add_library(nexilis SHARED ${SOURCES})

set(BOOST_LIBS
    json system assert align config core static_assert
    throw_exception container variant variant2 intrusive
    mp11 move container_hash winapi describe predef endian
)

foreach(lib ${BOOST_LIBS})
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third-party/boost/libs/${lib}/CMakeLists.txt)
        add_subdirectory(third-party/boost/libs/${lib})
    endif()
endforeach()

target_include_directories(nexilis PUBLIC include)

target_link_libraries(nexilis PUBLIC
    boost_json
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(nexilis PRIVATE
        ws2_32
        mswsock
    )
endif()
