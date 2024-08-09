cmake_minimum_required(VERSION 3.15)

# 项目名称
project(MyCProject)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/add)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/sub)

add_executable(main 
    main.c 
    add/add.c 
    sub/sub.c)
