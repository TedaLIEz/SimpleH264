cmake_minimum_required(VERSION 3.0)
project(SimpleH264)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall")


include_directories(include)
add_subdirectory(test)
set(H264_HEAD include)
set(SOURCE_FILES src/main.cpp src/nalu.cpp src/nalu_parser.cpp src/sps_parser.cpp)
add_executable(SimpleH264 ${H264_HEAD} ${SOURCE_FILES})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

include (TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -D__BIG_ENDIAN")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -D__LITTLE_ENDIAN")
endif()