cmake_minimum_required(VERSION 3.10)
project(threadpool_cpp)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找线程库
find_package(Threads REQUIRED)

# 添加TaskQueue库
add_library(TaskQueue STATIC TaskQueue/TaskQueue.cpp TaskQueue/TaskQueue.h)

# 添加ThreadPool库，并链接到TaskQueue
add_library(ThreadPool STATIC ThreadPool/ThreadPool.cpp ThreadPool/ThreadPool.h)
target_link_libraries(ThreadPool PRIVATE TaskQueue)

# 添加可执行文件，并链接到ThreadPool和线程库
add_executable(app main.cpp)
target_link_libraries(app PRIVATE ThreadPool TaskQueue Threads::Threads)

# 包含头文件目录
target_include_directories(TaskQueue PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/TaskQueue)
target_include_directories(ThreadPool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThreadPool)
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/TaskQueue ${CMAKE_CURRENT_SOURCE_DIR}/ThreadPool)
