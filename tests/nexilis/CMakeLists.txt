cmake_minimum_required(VERSION 3.16)
project(nexilis_tests)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(POLICY CMP0144)
    cmake_policy(SET CMP0144 NEW)
endif()

if (NEXILIS_IS_NIX)
    list(APPEND CMAKE_PREFIX_PATH "ENV{out}")
else()
    list(APPEND CMAKE_PREFIX_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}/../../nexilis/build")
endif()

message(STATUS "Nexilis paths: ${CMAKE_PREFIX_PATH}")

find_package(Nexilis REQUIRED CONFIG)
find_package(Boost 1.75 REQUIRED COMPONENTS json)
include(../ColorizedStatus.cmake)
print_nexilis_status()

find_package(GTest REQUIRED)

add_executable(nexilis_tests
    src/main.cc

    src/command_spec_test.cc
    src/room_spec_test.cc

    src/logging/logger_test.cc
    src/logging/log_test.cc

    src/protocol/protocol_manager_test.cc
    src/protocol/boost_tcp_test.cc
    src/protocol/protocol_test.cc
)

target_link_libraries(nexilis_tests PRIVATE
    Nexilis::nexilis
    GTest::GTest
    Boost::json
)

# Only add include dirs if not using the config file
if(NOT TARGET Nexilis::nexilis)
    target_include_directories(nexilis_tests PRIVATE
        ${CMAKE_PREFIX_PATH}/include
    )
endif()

install(TARGETS nexilis_tests DESTINATION bin)

