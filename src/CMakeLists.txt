cmake_minimum_required(VERSION 2.6)
project(Beamforming)


add_subdirectory(Beam_algo)
add_subdirectory(Calibrator)


list(APPEND BEAMFORMER_SOURCES
  Beamformer.cpp
  Beamformer.hpp
  )

list(APPEND BEAMFORMER_LINKS
  AD5371_CTRL_LIB
  CALIBRATION_LIB
  SIC_LIB
  READER_COMM_LIB
  BEAM_ALGO_COMMON_LIB
  BEAM_ALGO_LIB
  )


add_library(BEAMFORMER_LIB STATIC ${BEAMFORMER_SOURCES})
target_link_libraries(BEAMFORMER_LIB ${BEAMFORMER_LINKS})


list(APPEND BEAMFORMING_LIB_LIST
  BEAMFORMER_LIB
  )

set(BEAMFORMING_LIB_LIST ${BEAMFORMING_LIB_LIST} PARENT_SCOPE)
