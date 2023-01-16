cmake_minimum_required(VERSION 3.2)
project(WebServer)
 
 
#设定编译参数
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "Debug")

aux_source_directory(${CMAKE_SOURCE_DIR}/src SOURCE)

#设定头文件路径
include_directories(${CMAKE_SOURCE_DIR}/inc)

 
#生成可执行文件
add_executable(main   ${CMAKE_SOURCE_DIR}/main.cpp ${SOURCE})
