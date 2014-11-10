cmake_minimum_required(VERSION 2.8.4)
project(BigNumbersClion)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/cygdrive/d/everth/sources/github/universityTasks/otherProgramming/BigNumbersClion/build")

set(SOURCE_FILES main.cpp)
add_executable(BigNumbersClion ${SOURCE_FILES})