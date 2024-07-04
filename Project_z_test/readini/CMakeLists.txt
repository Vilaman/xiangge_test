cmake_minimum_required(VERSION 2.0)
project(readini CXX)

# 设置 C 和 C++ 标准
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR})

aux_source_directory(${PROJECT_SOURCE_DIR} SRC_LIST)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_library(readini SHARED ${SRC_LIST})

