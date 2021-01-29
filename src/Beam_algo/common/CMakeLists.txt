cmake_minimum_required(VERSION 2.6)
project(Beam_algo_common)

list(APPEND BEAM_COMMON_SOURCES
  CA_calculator.cpp
  CA_calculator.hpp
  Beam_util.cpp
  Beam_util.hpp
  Kalmaned_CA_calculator.cpp
  Kalmaned_CA_calculator.hpp
  )

list(APPEND BEAM_COMMON_LINKS
  armadillo
  KALMAN_FILTER_LIB
  )

add_library(BEAM_ALGO_COMMON_LIB STATIC ${BEAM_COMMON_SOURCES})
target_link_libraries(BEAM_ALGO_COMMON_LIB ${BEAM_COMMON_LINKS})
