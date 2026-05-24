cmake_minimum_required(VERSION 3.20)
project(set9_string_sorts CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
  add_compile_options(/O2 /W3)
else()
  add_compile_options(-O2 -Wall -Wextra)
endif()

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(bench src/main.cpp)

add_executable(cf_a1m src/cf/cf_a1m.cpp)
add_executable(cf_a1q src/cf/cf_a1q.cpp)
add_executable(cf_a1r src/cf/cf_a1r.cpp)
add_executable(cf_a1rq src/cf/cf_a1rq.cpp)
