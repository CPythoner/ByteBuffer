cmake_minimum_required(VERSION 3.5)
project(ByteBuffer VERSION 0.1.0)

# 排除 catch.hpp 和 ByteBuffer.h 从可执行文件编译
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx)

add_executable(${PROJECT_NAME} ${SOURCES})

# 启用测试
enable_testing()
add_test(NAME ByteBufferTest COMMAND ${PROJECT_NAME})
