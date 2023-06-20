cmake_minimum_required(VERSION 3.12)
project(example1)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Boost libraries
find_package(Boost REQUIRED COMPONENTS thread chrono)

# Include Boost directories
include_directories(${Boost_INCLUDE_DIRS})

add_compile_options(-Wall -Wextra -pedantic -static-libstdc++ -static-libgcc -pthread)

# Add the executable
add_executable(example1 main.cc)

# Link against Boost libraries
target_link_libraries(example1 ${Boost_LIBRARIES})
