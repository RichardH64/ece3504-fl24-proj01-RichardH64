cmake_minimum_required(VERSION 3.19)

project(myDisassembler VERSION 3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB HEADERS "${CMAKE_SOURCE_DIR}/src/*.hpp")

include_directories(${CMAKE_SOURCE_DIR})

# Define the executable
add_executable(myDisassembler ${SOURCES} ${HEADERS})

# Set output name and extension based on the target platform
if(CROSS_COMPILE_LINUX OR CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set_target_properties(myDisassembler PROPERTIES OUTPUT_NAME "myDisassembler" SUFFIX "")
elseif(CROSS_COMPILE_MACOS OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set_target_properties(myDisassembler PROPERTIES OUTPUT_NAME "myDisassembler" SUFFIX "")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(myDisassembler PROPERTIES OUTPUT_NAME "myDisassembler" SUFFIX ".exe")
endif()
