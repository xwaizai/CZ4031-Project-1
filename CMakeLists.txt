cmake_minimum_required(VERSION 3.00)
set(PROJECT_NAME projectname)
set(MAIN src/main.c)
project(${PROJECT_NAME})
set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ")

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/src")

add_executable(executable src/main.c)