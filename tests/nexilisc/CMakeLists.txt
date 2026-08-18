cmake_minimum_required(VERSION 3.25)
project(nexilis_c_tests)

if(POLICY CMP0144)
    cmake_policy(SET CMP0144 NEW)
endif()

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

file(GLOB_RECURSE NEXILIS_C_TEST_SOURCES CONFIGURE_DEPENDS "src/*.cc")

add_executable(nexilis_c_tests ${NEXILIS_C_TEST_SOURCES})

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

