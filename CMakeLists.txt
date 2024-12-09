cmake_minimum_required(VERSION 3.7)

# -- build the library
project(linalg LANGUAGES C)

set(OpenCL_VERSION 1.2)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0")
endif()

add_library(linalg STATIC
    vector.c
    matrix.c
    errors.c
    util.c
    linsolve.c
    eigen.c
    linreg.c
    rand.c
    kernel.c
)
set_target_properties(linalg PROPERTIES PUBLIC_HEADER
    eigen.h
    errors.h
    linalg_obj.h
    linreg.h
    linsolve.h
    matrix.h
    rand.h
    util.h
    vector.h
    kernel.h
    kernels.cl
)

set_property(TARGET linalg PROPERTY POSITION_INDEPENDENT_CODE ON)

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})

target_compile_features(linalg PRIVATE c_std_99)
target_link_libraries(linalg PUBLIC ${OpenCL_LIBRARIES})

# -- build the test
project(test LANGUAGES C)

add_executable(test main.c tests.c)
target_compile_features(test PRIVATE c_std_90)
target_link_libraries(test linalg m) # m = math lib



