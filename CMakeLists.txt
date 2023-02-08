cmake_minimum_required(VERSION 3.0.0)
set(PROJECT_NAME SUPERDB)
set(MAIN src/main.c)
project(${PROJECT_NAME})
set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB src_files CONFIGURE_DEPENDS
     "include/*.h"
     "src/*.c"
)

add_executable(${PROJECT_NAME} ${src_files})