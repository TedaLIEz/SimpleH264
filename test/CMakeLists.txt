cmake_minimum_required(VERSION 3.0)

project(SimpleH264)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall")

add_subdirectory(googletest/googletest)

include_directories(../include unit)

set(C_PARSER_HEAD
        ${gtest_SOURCE_DIR}
        ../include
        )

set(C_PARSER_SRC
        unit/test_main.cpp
        )

add_executable(SimpleH264_Test ${C_PARSER_SRC} ${C_PARSER_HEAD})

target_link_libraries(SimpleH264_Test gtest)