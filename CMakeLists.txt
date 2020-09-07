cmake_minimum_required(VERSION 2.8)

include_directories(${PROJECT_SOURCE_DIR}/lib)
include_directories(${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_BINARY_DIR}/src)

include_directories(${PROJECT_SOURCE_DIR}/global)
include_directories(${PROJECT_BINARY_DIR}/global)

list(APPEND BEAMFORMING_LIB_LIST
  beamforming
  )

add_subdirectory(src)
add_subdirectory(lib)

message(STATUS "hello ${BEAMFORMING_LIB_LIST}")

add_executable(beamforming src/beamforming.cpp)
target_link_libraries(${BEAMFORMING_LIB_LIST})
