cmake_minimum_required(VERSION 2.8.9)
project(node-libproxy)

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH})
link_directories(${CMAKE_LIBRARY_PATH})
find_library(LIPROXY_LIB libproxy)
add_library(LIBPROXY_LIB STATIC IMPORTED)

add_executable(node node.cpp)
target_link_libraries(node LIPROXY_LIB)
