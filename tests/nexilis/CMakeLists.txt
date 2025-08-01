cmake_minimum_required(VERSION 3.13)
project(nexilis_tests)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

if (IS_NIX_BUILD)
    message(STATUS "Using installed Nexilis from: ${CMAKE_PREFIX_PATH}")
    include_directories(${CMAKE_PREFIX_PATH}/include)
    link_directories(${CMAKE_PREFIX_PATH}/lib)
else()
    message(STATUS "Using local Nexilis source via add_subdirectory")
    add_subdirectory(../../nexilis/ nexilis_dir)
endif()

add_compile_options(-Wall -Wextra -pedantic -Wconversion)

add_executable(nexilis_tests
    src/main.cc

    src/logging/logger_test.cc
    src/logging/log_test.cc

    src/protocol/protocol_manager_test.cc
    src/protocol/boost_tcp_test.cc
    src/protocol/protocol_test.cc
)

target_link_libraries(nexilis_tests nexilis ${GTEST_LIBRARIES})
install(TARGETS nexilis_tests DESTINATION bin)

