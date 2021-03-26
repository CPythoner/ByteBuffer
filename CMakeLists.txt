cmake_minimum_required(VERSION 3.0.0)
project(ByteBuffer VERSION 0.1.0)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR} *.c *.cpp *.cxx *.h *.hpp)

add_executable(${PROJECT_NAME} ${SOURCES})
