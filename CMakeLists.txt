cmake_minimum_required(VERSION 2.8)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_BINARY_DIR}/src)

list(APPEND BEAMFORMING_LIB_LIST
  beamforming
  cxxopts
  )

list(APPEND CALIBRATOR_LIB_LIST
  calibrator
  cxxopts
  )

list(APPEND CALIBRATOR_LINKS
  CALIBRATION_LIB
  )


add_subdirectory(src)
add_subdirectory(lib)

message(STATUS "hello ${BEAMFORMING_LIB_LIST}")

add_executable(beamforming src/beamforming_main.cpp)
add_executable(calibrator src/calibrator_main.cpp)

target_link_libraries(${BEAMFORMING_LIB_LIST})
target_link_libraries(calibrator ${CALIBRATOR_LINKS})
