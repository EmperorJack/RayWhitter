cmake_minimum_required(VERSION 3.7)
project(RayWhitter)

set(CMAKE_CXX_STANDARD 11)

# Include source files
include_directories(${PROJECT_SOURCE_DIR}/src)
file(GLOB SOURCE_FILES
    src/*.hpp
    src/*.cpp
)

# Include the glm header library
include_directories(${PROJECT_SOURCE_DIR}/lib/glm)

# Include the jpeg-compressor library
include_directories(${PROJECT_SOURCE_DIR}/lib/jpeg-compressor)

# Include any external library files
file(GLOB EXTERNAL_FILES
    lib/glm/glm/glm.hpp
    lib/jpeg-compressor/jpge.h
    lib/jpeg-compressor/jpge.cpp
)

add_executable(RayWhitter ${SOURCE_FILES} ${EXTERNAL_FILES})