cmake_minimum_required(VERSION 2.6)
project(Beam_algo)

list(APPEND BEAM_SOURCES
  Beamtrainer.cpp
  Beamtrainer.hpp
  Random_beamtrainer.cpp
  Random_beamtrainer.hpp
  Directional_beamtrainer.cpp
  Directional_beamtrainer.hpp
  CA_with_directional_beamtrainer.cpp
  CA_with_directional_beamtrainer.hpp
  Beam_algorithm.cpp
  Beam_algorithm.hpp
  )

list(APPEND BEAM_LINKS
  armadillo
  )

add_library(BEAM_ALGO_LIB STATIC ${BEAM_SOURCES})
target_link_libraries(BEAM_ALGO_LIB ${BEAM_LINKS})

list(APPEND BEAMFORMING_LIB_LIST
  BEAM_ALGO_LIB
  )

set(BEAMFORMING_LIB_LIST ${BEAMFORMING_LIB_LIST} PARENT_SCOPE)
