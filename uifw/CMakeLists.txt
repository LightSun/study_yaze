#sudo apt install libwavpack-dev
cmake_minimum_required(VERSION 3.10)

project(uifw VERSION 0.01)

# C++ Standard Specifications
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_MODULE_LINKER_FLAGS \"-Wl,--no-undefined -Wl,--no-undefined\")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(BUILD_SHARED_LIBS ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Abseil Standard Specifications
set(EXT_DIR ../yaze)
include(cmake/absl.cmake)
add_subdirectory(${EXT_DIR}/src/lib/abseil-cpp absl)

include(cmake/openssl.cmake)
# Video Libraries
find_package(PNG REQUIRED)
find_package(OpenGL REQUIRED)
include(cmake/sdl2.cmake)
# ImGui
include(cmake/imgui.cmake)

#
include_directories(src/)
