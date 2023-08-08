cmake_minimum_required(VERSION 3.14)
project(socket)
include(build/arch/add_definite.cmake)

include_directories(/usr/include)
# 仅仅为了 把 thrift 的头文件 下载下来
#include_directories(/usr/include/thrift)
add_subdirectory(mac)
add_subdirectory(rpcserver)