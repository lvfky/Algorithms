cmake_minimum_required(VERSION 3.16)
project(hll_study CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(experiment
  src/experiment_main.cpp
  src/random_stream_gen.cpp
  src/hash_func_gen.cpp
  src/hll.cpp
  src/hll_plus.cpp
)
add_executable(hash_uniformity
  src/hash_uniformity_main.cpp
  src/random_stream_gen.cpp
  src/hash_func_gen.cpp
)