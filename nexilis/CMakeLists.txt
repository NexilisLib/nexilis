cmake_minimum_required(VERSION 3.25)
project(nexilis)

set(NEXILIS_CXX_STANDARD 17)

string(TOLOWER "${CMAKE_SYSTEM_NAME}" NEXILIS_OS_RAW)
if(NEXILIS_OS_RAW MATCHES "windows")
    set(NEXILIS_OS "Windows")
elseif(NEXILIS_OS_RAW MATCHES "linux")
    set(NEXILIS_OS "Linux")
elseif(NEXILIS_OS_RAW MATCHES "darwin")
    set(NEXILIS_OS "Darwin")
else()
    set(NEXILIS_OS "${CMAKE_SYSTEM_NAME}")
endif()

set(NEXILIS_C_COMPILER "${CMAKE_C_COMPILER_ID}")
set(NEXILIS_C_COMPILER_VERSION "${CMAKE_C_COMPILER_VERSION}")
set(NEXILIS_CXX_COMPILER "${CMAKE_CXX_COMPILER_ID}")
set(NEXILIS_CXX_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
set(NEXILIS_ARCH "${CMAKE_SYSTEM_PROCESSOR}")

message(STATUS "System Information:")
message(STATUS "  OS: ${NEXILIS_OS}")
message(STATUS "  Arch: ${NEXILIS_ARCH}")
message(STATUS "  C Compiler: ${NEXILIS_C_COMPILER} ${NEXILIS_C_COMPILER_VERSION}")
message(STATUS "  C++ Compiler: ${NEXILIS_CXX_COMPILER} ${NEXILIS_CXX_COMPILER_VERSION}")
message(STATUS "  C++ Standard: ${NEXILIS_CXX_STANDARD}")

# Setting default if not specified via -DCMAKE_BUILD_TYPE.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug or Release)" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(IS_CI FALSE CACHE BOOL "True if running in CI environment")
set(IS_NIX_BUILD FALSE CACHE BOOL "True if running under Nix build")
set(IS_LOCAL_BUILD FALSE CACHE BOOL "True if running locally")
set(USE_SUBMODULES FALSE CACHE BOOL "True if submodules should be used")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(BUILD_SHARED_LIBS ON)

# Compile with clang.
#set(CMAKE_C_COMPILER "clang")
#set(CMAKE_CXX_COMPILER "clang++")

function(configure_build_environment)
    set(CI_VARS
        "GITHUB_ACTIONS"
        "GITLAB_CI"
        "CI"
        "TRAVIS"
        "CIRCLECI"
    )

    # Check for CI.
    foreach(var ${CI_VARS})
        if(DEFINED ENV{${var}})
            set(IS_CI TRUE CACHE BOOL "True if running in CI environment" FORCE)
            set(USE_SUBMODULES TRUE CACHE BOOL "True if submodules should be used" FORCE)
            set(IS_LOCAL_BUILD FALSE CACHE BOOL "True if running locally")
            return()
        endif()
    endforeach()

    # Check for Nix build environment.
    if(DEFINED ENV{NIX_BUILD_CORES})
        set(IS_NIX_BUILD TRUE CACHE BOOL "True if running under Nix build" FORCE)
        set(USE_SUBMODULES FALSE CACHE BOOL "True if submodules should be used" FORCE)
        set(IS_LOCAL_BUILD FALSE CACHE BOOL "True if running locally" FORCE)
        return()
    endif()

    # Use submodules locally.
    set(IS_LOCAL_BUILD TRUE CACHE BOOL "True if running locally" FORCE)
    set(IS_CI FALSE CACHE BOOL "True if running in CI environment" FORCE)
    set(IS_NIX_BUILD FALSE CACHE BOOL "True if running under Nix build" FORCE)
    set(USE_SUBMODULES TRUE CACHE BOOL "True if submodules should be used" FORCE)
endfunction()

configure_build_environment()

message(STATUS "Build configuration:")
message(STATUS "  CI Environment: ${IS_CI}")
message(STATUS "  Nix Build: ${IS_NIX_BUILD}")
message(STATUS "  Local Build: ${IS_LOCAL_BUILD}")
message(STATUS "  Use Submodules: ${USE_SUBMODULES}")

if (NEXILIS_OS STREQUAL "Windows")
    if (MSVC)
        add_compile_options(/W4 /WX)
    endif()
elseif (NEXILIS_OS STREQUAL "Darwin")
    add_compile_options(-static-libstdc++ -static-libgcc -pthread)
elseif (NEXILIS_OS STREQUAL "Linux")
    add_compile_options(-Wall -Wextra -pedantic -pthread)
endif()

if (IS_CI)
    add_compile_definitions(NEXILIS_IS_CI=1)
else()
    add_compile_definitions(NEXILIS_IS_CI=0)
endif()

if (IS_NIX_BUILD)
    add_compile_definitions(NEXILIS_IS_NIX=1)
else()
    add_compile_definitions(NEXILIS_IS_NIX=0)
endif()

if (IS_LOCAL_BUILD)
    add_compile_definitions(NEXILIS_IS_LOCAL=1)
else()
    add_compile_definitions(NEXILIS_IS_LOCAL=0)
endif()

if (USE_SUBMODULES)
    add_compile_definitions(NEXILIS_USE_SUBMODULES=1)
else()
    add_compile_definitions(NEXILIS_USE_SUBMODULES=0)
endif()

if (USE_SUBMODULES)
    message(STATUS "Using boost submodules")
    set(BOOST_SUBMODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third-party/boost")

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
else()
    message(STATUS "Using system boost")
    find_package(Boost 1.75 REQUIRED COMPONENTS json system)

    if(NOT Boost_FOUND)
        message(FATAL_ERROR, "System boost not found and submodules missing")
    endif()
endif()

set(SOURCES
    # Base
    src/nexilis/environment.cc
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
    src/nexilis/client/protocol_status.cc

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
target_include_directories(nexilis PUBLIC include)

if (USE_SUBMODULES)
    target_link_libraries(nexilis PUBLIC
        boost_json
    )
else()
    target_include_directories(nexilis PUBLIC ${Boost_INCLUDE_DIRS})
    target_link_libraries(nexilis PUBLIC
        Boost::json
    )
endif()

if (NEXILIS_OS STREQUAL "Windows")
    target_link_libraries(nexilis PRIVATE
        ws2_32
        mswsock
    )
endif()
