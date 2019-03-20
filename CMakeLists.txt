cmake_minimum_required(VERSION 3.13)

project(TARget)

# use C++17
# --------------------------------------- #
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# libraries
# --------------------------------------- #
include_directories(include)

# source
# --------------------------------------- #
file(GLOB SRC_TARGET "src/*.cpp")

# executables
# --------------------------------------- #
add_executable(target ${SRC_TARGET})