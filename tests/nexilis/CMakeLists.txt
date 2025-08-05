cmake_minimum_required(VERSION 3.16)
project(nexilis_tests)

if(POLICY CMP0144)
    cmake_policy(SET CMP0144 NEW)
endif()

string(ASCII 27 Esc)
set(ColorReset "${Esc}[m")
set(ColorBold "${Esc}[1m")
set(ColorGreen "${Esc}[32m")
set(ColorRed "${Esc}[31m")

if (NEXILIS_IS_NIX)
    list(APPEND CMAKE_PREFIX_PATH "ENV{out}")
else()
    list(APPEND CMAKE_PREFIX_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}/../../nexilis/build")
endif()

message(STATUS "Nexilis paths: ${CMAKE_PREFIX_PATH}")

find_package(Nexilis REQUIRED CONFIG)

function(colorized_status label value)
    if(value)
        set(color ${ColorGreen})
        set(display_value "ON")
    else()
        set(color ${ColorRed})
        set(display_value "OFF")
    endif()
    message(STATUS "${ColorBold} ${label}:${ColorReset} ${color}${display_value}${ColorReset}")
endfunction()

message(STATUS "${ColorBold} Version: ${NEXILIS_VERSION}")
colorized_status("Nix Environment" ${NEXILIS_IS_NIX})
colorized_status("Local Development" ${NEXILIS_IS_LOCAL})
colorized_status("CI Environment" ${NEXILIS_IS_CI})
colorized_status("Using Submodules" ${NEXILIS_USE_SUBMODULES})

find_package(GTest REQUIRED)

add_executable(nexilis_tests
    src/main.cc

    src/logging/logger_test.cc
    src/logging/log_test.cc

    src/protocol/protocol_manager_test.cc
    src/protocol/boost_tcp_test.cc
    src/protocol/protocol_test.cc
)

target_link_libraries(nexilis_tests PRIVATE
    Nexilis::nexilis
    GTest::GTest
)

# Only add include dirs if not using the config file
if(NOT TARGET Nexilis::nexilis)
    target_include_directories(nexilis_tests PRIVATE
        ${CMAKE_PREFIX_PATH}/include
    )
endif()

install(TARGETS nexilis_tests DESTINATION bin)

