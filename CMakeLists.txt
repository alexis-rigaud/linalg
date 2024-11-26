cmake_minimum_required(VERSION 3.25)
project(linalg)

## set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_C_STANDARD 99)
set(OpenCL_VERSION 1.2)

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})

## -- build
set (SRC vector.c matrix.c errors.c util.c tests.c linsolve.c eigen.c linreg.c rand.c)
add_executable(linalg main.c ${SRC})

target_link_libraries(linalg ${OpenCL_LIBRARIES})

