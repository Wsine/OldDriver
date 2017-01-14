cmake_minimum_required(VERSION 2.8)
project(OldDriver)

find_package(Boost 1.54 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR}/src/)

add_library(controlcan SHARED IMPORTED)
if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set_property(TARGET controlcan PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/lib64/libcontrolcan.so)
else ()
    set_property(TARGET controlcan PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/lib32/libcontrolcan.so)
endif()

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(OldDriver ${SRC_FILES})
target_link_libraries(OldDriver pthread rt controlcan lcm)

add_custom_target(
    GenLCM 
    COMMAND lcm-gen -x Controller.lcm
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/communication/
)
add_dependencies(OldDriver GenLCM)
