cmake_minimum_required(VERSION 3.5)

set(TARGET muenvsubst)

project(${TARGET} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_link_options(-static -static-libgcc -static-libstdc++ -s)
endif()

add_subdirectory(cli)
