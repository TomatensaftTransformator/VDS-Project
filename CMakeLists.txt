cmake_minimum_required(VERSION 2.8)
project(VDSProject CXX C)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC -std=c++11 ${LLVM_CXX_FLAGS} ")




add_subdirectory(src/external)  #gtest
add_subdirectory(src/verify)    #verify
add_subdirectory(src/bench)    #bench
add_subdirectory(src/reachable)    #reachable

#Include dirs
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_SOURCE_DIR}/src)

include_directories(${CMAKE_SOURCE_DIR}/src/test/)
link_directories(${CMAKE_SOURCE_DIR}/src/test/)


include_directories(${CMAKE_SOURCE_DIR}/src/verify/)
link_directories(${CMAKE_SOURCE_DIR}/src/verify/)


include_directories(${CMAKE_SOURCE_DIR}/src/bench/)
link_directories(${CMAKE_SOURCE_DIR}/src/bench/)


include_directories(${CMAKE_SOURCE_DIR}/src/reachable/)
link_directories(${CMAKE_SOURCE_DIR}/src/reachable/)


add_subdirectory(src)
add_executable(VDSProject src/main.cpp)
target_link_libraries(VDSProject Manager)
