include_directories(${CMAKE_SOURCE_DIR}/add/include)
include_directories(${CMAKE_SOURCE_DIR}/sub)
include_directories(${CMAKE_DIR}/build/cmake)

message(STATUS "*********include_directories：${PROJECT_SOURCE_DIR}/add/include")

add_executable(main
            main.cpp)

target_link_libraries(main PUBLIC
            add
            sub
            mydata)         