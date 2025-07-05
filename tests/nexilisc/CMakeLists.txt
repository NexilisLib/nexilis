cmake_minimum_required(VERSION 3.25)
project(nexilis_c_tests)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(../../nexilis/ nexilis_dir)

add_compile_options(-Wall -Wextra -pedantic -Wconversion)

add_executable(nexilis_c_tests
    src/main.cc

    src/logging/logger_test_c.cc
    src/logging/log_test_c.cc

    src/protocol/protocol_manager_test_c.cc
    src/protocol/protocol_test_c.cc
)

target_link_libraries(nexilis_c_tests nexilis ${GTEST_LIBRARIES})
