cmake_minimum_required(VERSION 3.28)
project("GEP")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CAMAKE_BINARY_DIR}/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")

set(ASSETS_INPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/assets")
set(SHADERS_INPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/shaders")

add_subdirectory("vendor")
add_subdirectory("src")
