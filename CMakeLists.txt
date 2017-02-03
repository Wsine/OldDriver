cmake_minimum_required(VERSION 2.8)
project(OldDriver)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR}/src/)

##################### Source Binary ########################

add_library(controlcan SHARED IMPORTED)
if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set_property(TARGET controlcan PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/lib64/libcontrolcan.so)
else()
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

##################### Test Binary ########################

add_definitions(-DBOOST_TEST_DYN_LINK)
file(GLOB_RECURSE TEMP_FILES ${PROJECT_SOURCE_DIR}/test/*.[c,h]pp)
foreach(t ${TEMP_FILES})
    get_filename_component(ext ${t} EXT)
    if(${ext} STREQUAL ".hpp")
        string(REPLACE "/test/" "/src/" t ${t})
        string(REPLACE "Test.hpp" ".cpp" t ${t})
    endif()
    list(APPEND TEST_FILES ${t})
endforeach(t ${TEMP_FILES})
add_executable(OldDriverTest ${TEST_FILES})
target_link_libraries(OldDriverTest ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} pthread rt controlcan lcm)

enable_testing()
add_test(NAME OldDriverTest COMMAND OldDriverTest)
