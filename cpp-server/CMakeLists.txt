cmake_minimum_required(VERSION 3.10)
project(CppMultiUserServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(server ${SOURCES})