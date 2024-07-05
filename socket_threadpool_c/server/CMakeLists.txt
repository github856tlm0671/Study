# 规定最小所需要的cmake版本
cmake_minimum_required(VERSION 3.10)

project(THREAD_POOL)

set(CMAKE_BUILD_TYPE Release)

include_directories(include)

add_executable(server server.c src/threadpool.c)

target_link_libraries(server pthread)
