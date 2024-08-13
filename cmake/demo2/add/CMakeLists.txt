include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
message(STATUS "SOURCE_DIR: ${SOURCE_DIR}")
add_library(add  ${SOURCE_DIR}/add.cpp)       