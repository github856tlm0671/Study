#!/bin/bash

# 设置项目根目录
PROJECT_ROOT="$(dirname "$(realpath "$0")")"

# 设置构建目录
BUILD_DIR="$PROJECT_ROOT/build"

# 如果构建目录存在，则删除它
if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
fi

# 创建构建目录
mkdir -p "$BUILD_DIR"

# 如果构建目录不存在，则创建它
if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
fi

# 进入构建目录
cd "$BUILD_DIR" || { echo "build文件创建失败！"; exit 1; }

# 运行 cmake 配置
cmake ..
if [ $? -eq 0 ]; then
    echo "CMake 配置成功！"
else
    echo "CMake 配置失败！"echo
    exit 1
fi

# 运行 make 构建
make VERBOSE=1
if [ $? -eq 0 ]; then
    echo "构建成功！"
else
    echo "错误：构建失败！"echo
    exit 1
fi




