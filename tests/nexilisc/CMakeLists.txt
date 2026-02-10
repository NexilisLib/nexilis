cmake_minimum_required(VERSION 3.25)
project(nexilis_c_tests)

if (NEXILIS_IS_NIX)
    list(APPEND CMAKE_PREFIX_PATH "ENV{out}")
else()
    list(APPEND CMAKE_PREFIX_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}/../../nexilis/build")
endif()

message(STATUS "Nexilis paths:  ${CMAKE_PREFIX_PATH}")

find_package(Nexilis REQUIRED CONFIG)
include(../ColorizedStatus.cmake)
print_nexilis_status()

find_package(GTest REQUIRED)

add_executable(nexilis_c_tests
    src/main.cc

    src/logging/logger_test_c.cc
    src/logging/log_test_c.cc

    src/protocol/protocol_manager_test_c.cc
    src/protocol/protocol_test_c.cc
)

target_link_libraries(nexilis_c_tests PRIVATE
    Nexilis::nexilisc
    GTest::GTest
)

# Only add include dirs if not using the config file
if(NOT TARGET Nexilis::nexilis)
    target_include_directories(nexilis_c_tests PRIVATE
        ${CMAKE_PREFIX_PATH}/include
    )
endif()

install(TARGETS nexilis_c_tests DESTINATION bin)

