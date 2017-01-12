cmake_minimum_required(VERSION 2.8)
project(OldDriver)

find_package(Boost 1.54 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
include_directories(.)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(OldDriver ${SRC_FILES})
target_link_libraries(OldDriver pthread rt)
