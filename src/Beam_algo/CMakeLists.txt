cmake_minimum_required(VERSION 2.6)
project(Beam_algo)

add_subdirectory(common)


list(APPEND BEAM_SOURCES
  Beamtrainer.cpp
  Beamtrainer.hpp
  Random_beamtrainer.cpp
  Random_beamtrainer.hpp
  Directional_beamtrainer.cpp
  Directional_beamtrainer.hpp
  Directional_with_refining_beamtrainer.cpp
  Directional_with_refining_beamtrainer.hpp
  Directional_with_refining_beamtrainer2.cpp
  Directional_with_refining_beamtrainer2.hpp
  CA_with_directional_beamtrainer.cpp
  CA_with_directional_beamtrainer.hpp
  CA_with_random_beamtrainer.cpp
  CA_with_random_beamtrainer.hpp
  Kalmaned_with_random_beamtrainer.cpp
  Kalmaned_with_random_beamtrainer.hpp
  Agent_beamtrainer.cpp
  Agent_beamtrainer.hpp
  Beam_algorithm.cpp
  Beam_algorithm.hpp
  Fixed_beamtrainer.cpp
  Fixed_beamtrainer.hpp
  Test_beamtrainer.cpp
  Test_beamtrainer.hpp
  )

list(APPEND BEAM_LINKS
  armadillo
  KALMAN_FILTER_LIB
  BEAM_ALGO_COMMON_LIB 
  )

add_library(BEAM_ALGO_LIB STATIC ${BEAM_SOURCES})
target_link_libraries(BEAM_ALGO_LIB ${BEAM_LINKS})

list(APPEND BEAMFORMING_LIB_LIST
  BEAM_ALGO_LIB
  )

set(BEAMFORMING_LIB_LIST ${BEAMFORMING_LIB_LIST} PARENT_SCOPE)
