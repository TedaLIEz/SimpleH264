cmake_minimum_required(VERSION 3.0)

project(SimpleH264)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall")

add_subdirectory(googletest)

include_directories(../include unit)

set(C_PARSER_HEAD
        ${gmock_SOURCE_DIR}
        ../include
        )

set(C_PARSER_SRC
        unit/test_main.cpp
        unit/NAL_test.cpp
        ../src/nalu_parser.cpp
        )

add_executable(SimpleH264_Test ${C_PARSER_SRC} ${C_PARSER_HEAD})

target_link_libraries(SimpleH264_Test gmock)

include (TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -D__BIG_ENDIAN")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -D__LITTLE_ENDIAN")
endif()