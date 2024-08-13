find_package(Protobuf CONFIG REQUIRED)

set(SOURCES mydata.proto)

add_library(mydata ${SOURCES})

protobuf_generate(TARGET mydata LANGUAGE CPP)

# Module模式就是使用库文件的路径链接
# find_package(Protobuf REQUIRED)
# target_link_libraries(mydata PUBLIC
#             ${Protobuf_LIBRARIES})

# Config模式就是使用目标来链接库
# find_package(Protobuf CONFIG REQUIRED)
target_link_libraries(mydata PUBLIC
            protobuf::libprotobuf) 
            
target_include_directories(mydata PUBLIC
            ${Protobuf_INCLUDE_DIRS}
            ${CMAKE_CURRENT_BINARY_DIR})
            


# find_package(Protobuf REQUIRED)
# # message(STATUS "Protobuf libraries: ${Protobuf_LIBRARIES}")


# set(PROTO_FILES mydata.proto)

# # 创建库
# add_library(mydata ${PROTO_FILES})


# # 生成 Protobuf C++ 文件
# protobuf_generate(TARGET mydata LANGUAGE cpp)

# # 链接 Protobuf 库
# target_link_libraries(mydata PUBLIC ${Protobuf_LIBRARIES})

# # 设置包含目录
# target_include_directories(mydata PUBLIC
#     ${Protobuf_INCLUDE_DIRS}
#     ${CMAKE_CURRENT_BINARY_DIR})
