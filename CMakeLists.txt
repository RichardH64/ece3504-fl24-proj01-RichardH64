cmake_minimum_required(VERSION 3.19)

project(myDisassembler VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/*.cpp")
file(GLOB HEADERS "${CMAKE_SOURCE_DIR}/*.hpp")

include_directories(${CMAKE_SOURCE_DIR})

add_executable(myDisassembler ${SOURCES} ${HEADERS})
