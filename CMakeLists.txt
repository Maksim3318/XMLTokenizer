cmake_minimum_required(VERSION 3.10)
project(XMLTokenizer)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(samples)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
