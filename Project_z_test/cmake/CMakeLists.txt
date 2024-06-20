#[[cmake_minimum_required (VERSION 2.8)

project (cmake)

add_subdirectory (src)]]

#cmake_minimum_required：指定使用的 cmake 的最低版本
cmake_minimum_required (VERSION 2.8)
# project：定义工程名称，并可指定工程的版本、工程描述、web主页地址、支持的语言
#（默认情况支持所有语言），如果不需要这些都是可以忽略的，只需要指定出工程名字即可
project (cmake)

set(CMAKE_CXX_STANDARD 11)
# PROJECT_SOURCE_DIR == cmake .. 指定的路径
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# CMAKE_CURRENT_SOURCE_DIR == cmakelists.txt 所在的路径
aux_source_directory (src SRC_LIST)
# LIBRARY_OUTPUT_PATH == 输出库路径
include_directories (include)

add_executable (main ${SRC_LIST})