#***************CMakeLists.txt*********************
# set (CMAKE_C_COMPILER "aarch64-linux-gnu-gcc")
# set (CMAKE_CXX_COMPILER "aarch64-linux-gnu-g++") #c++编译使用这个
# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.16)

# 设置工程名
project (CANopen_Linux)

set(CMAKE_C_FLAGS -lpthread) #编译命令后缀 因为使用了线程，需要添加-lpthread
set(CMAKE_CXX_FLAGS -lpthread)#c++使用这个

include_directories (main CANopen/inc CANopen/inc/linux )#包含头文件目录
include_directories (CANopen/dictionary CANopen/hardware)#包含头文件目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory(${PROJECT_SOURCE_DIR}/CANopen/src CANOPEN_SRC)#添加canopen源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/CANopen/hardware CANOPEN_HARDWARE)#添加硬件
aux_source_directory(${PROJECT_SOURCE_DIR}/CANopen/dictionary CANOPEN_DICTIONARY)#添加词典

aux_source_directory(${PROJECT_SOURCE_DIR}/main MAIN_SRCS)#添加main
# 指定生成目标canopen_exe
add_executable(canopen_exe ${MAIN_SRCS} ${CANOPEN_HARDWARE} ${CANOPEN_SRC} ${CANOPEN_DICTIONARY})

target_link_libraries(canopen_exe -pthread)
