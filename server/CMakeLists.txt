cmake_minimum_required(VERSION 3.5)
project(GameServer)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(CMAKE_CXX_FLAGS "--std=c++14 -pthread -Wall -pedantic -Wextra")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src)

enable_testing()

add_subdirectory(tests)
