cmake_minimum_required(VERSION 3.25)
project(nexilis)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# VERSION.txt versioning
file(STRINGS "${CMAKE_SOURCE_DIR}/../VERSION.txt" PROJECT_VERSION_RAW)
string(STRIP "${PROJECT_VERSION_RAW}" PROJECT_VERSION_RAW)
string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)(-[a-zA-Z0-9]+)?" VERSION_PARTS "${PROJECT_VERSION_RAW}")

set(NEXILIS_VERSION_MAJOR ${CMAKE_MATCH_1})
set(NEXILIS_VERSION_MINOR ${CMAKE_MATCH_2})
set(NEXILIS_VERSION_PATCH ${CMAKE_MATCH_3})
set(NEXILIS_VERSION_SUFFIX ${CMAKE_MATCH_4})
set(NEXILIS_VERSION "${NEXILIS_VERSION_MAJOR}.${NEXILIS_VERSION_MINOR}.${NEXILIS_VERSION_PATCH}${NEXILIS_VERSION_SUFFIX}")
set(NEXILIS_VERSION_FULL "${NEXILIS_VERSION}")

message(STATUS "System Information:")
message(STATUS "  OS: ${CMAKE_SYSTEM_NAME}")
message(STATUS "  Arch: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "  C Compiler: ${CMAKE_C_COMPILER} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "  C++ Compiler: ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")

# Setting default if not specified via -DCMAKE_BUILD_TYPE.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug or Release)" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(BUILD_SHARED_LIBS ON)

function(configure_build_environment)
    # Nix devshell.
    if(DEFINED ENV{NIX_BUILD_CORES})
        set(USE_SUBMODULES 0 PARENT_SCOPE)
        set(IS_NIX 1 PARENT_SCOPE)
        set(IS_LOCAL 0 PARENT_SCOPE)
        set(IS_CI 0 PARENT_SCOPE)
        return()
    endif()

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
            set(USE_SUBMODULES 0 PARENT_SCOPE)
            set(IS_CI 1 PARENT_SCOPE)
            set(IS_LOCAL 0 PARENT_SCOPE)
            set(IS_NIX 0 PARENT_SCOPE)
            return()
        endif()
    endforeach()

    # Use submodules locally.
    set(USE_SUBMODULES 1 PARENT_SCOPE)
    set(IS_LOCAL 1 PARENT_SCOPE)
    set(IS_CI 0 PARENT_SCOPE)
    set(IS_NIX 0 PARENT_SCOPE)
endfunction()

configure_build_environment()

set(NEXILIS_IS_CI ${IS_CI} CACHE BOOL "Whether this is a CI build" FORCE)
set(NEXILIS_IS_NIX ${IS_NIX} CACHE BOOL "Whether this is a Nix build" FORCE)
set(NEXILIS_IS_LOCAL ${IS_LOCAL} CACHE BOOL "Whether this is a local build" FORCE)
set(NEXILIS_USE_SUBMODULES ${USE_SUBMODULES} CACHE BOOL "Whether to use submodules" FORCE)

message(STATUS "Build configuration:")
message(STATUS "  CI Environment: ${NEXILIS_IS_CI}")
message(STATUS "  Nix Build: ${NEXILIS_IS_NIX}")
message(STATUS "  Local Build: ${NEXILIS_IS_LOCAL}")
message(STATUS "  Use Submodules: ${NEXILIS_USE_SUBMODULES}")

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if (MSVC)
        add_compile_options(/W4 /WX)
    endif()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_compile_options(-static-libstdc++ -static-libgcc -pthread)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_options(-Wall -Wextra -pedantic -pthread)
endif()

add_compile_definitions(
    NEXILIS_IS_CI=${IS_CI}
    NEXILIS_IS_NIX=${IS_NIX}
    NEXILIS_IS_LOCAL=${IS_LOCAL}
    NEXILIS_USE_SUBMODULES=${USE_SUBMODULES}
)

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
    find_package(Boost 1.75 REQUIRED COMPONENTS json)

    if(NOT Boost_FOUND)
        message(FATAL_ERROR, "System boost not found and submodules missing")
    endif()

    # Create imported targets for system Boost
    if(NOT TARGET Boost::json)
        if (NOT TARGET BOOST::json)
            add_library(Boost::json ALIAS Boost::boost_json)
        else()
            add_library(Boost::json UNKNOWN IMPORTED)
            set_target_properties(Boost::json PROPERTIES
                IMPORTED_LOCATION "${Boost_JSON_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}"
                INTERFACE_LINK_LIBRARIES "Boost::headers"
            )
        endif()
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

    # BoostTCP specific
    src/nexilis/boost_tcp/socket.cc

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
target_include_directories(nexilis PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(nexilis PUBLIC
    $<IF:$<BOOL:${USE_SUBMODULES}>,boost_json,Boost::json>
)

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(nexilis PRIVATE
        ws2_32
        mswsock
    )
endif()

install(TARGETS nexilis
    EXPORT NexilisTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

export(EXPORT NexilisTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/NexilisTargets.cmake"
    NAMESPACE Nexilis::
)

install(EXPORT NexilisTargets
    FILE NexilisTargets.cmake
    NAMESPACE Nexilis::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Nexilis
)

configure_package_config_file(
    cmake/NexilisConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/NexilisConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Nexilis
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/NexilisConfigVersion.cmake
    VERSION ${NEXILIS_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/NexilisConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/NexilisConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Nexilis
)
