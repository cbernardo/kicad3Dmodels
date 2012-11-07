# file: ${K3D_SOURCE_DIR}/src/vrml/pcc/CMakeLists.txt
#
# Model PCC-SMP-X horizontal mount thermocouple connector
#

include_directories(${K3D_SOURCE_DIR}/include/common ${K3D_SOURCE_DIR}/include/vrml/common) 

link_directories(${K3D_BINARY_DIR}/src/common ${K3D_BINARY_DIR}/src/mat
  ${K3D_BINARY_DIR}/src/vrml/vcom)

add_executable(makePCC-SMP pccsmpx.cpp)

target_link_libraries(makePCC-SMP vcom)

#install(TARGETS make19950 RUNTIME DESTINATION bin)
