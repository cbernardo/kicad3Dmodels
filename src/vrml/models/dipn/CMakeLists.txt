# file: ${K3D_SOURCE_DIR}/src/vrml/dipn/CMakeLists.txt
#
# Parametric model for DIPn packages
#

include_directories(${K3D_SOURCE_DIR}/include/common ${K3D_SOURCE_DIR}/include/vrml/common
  ${K3D_SOURCE_DIR}/include/vrml/models) 

link_directories(${K3D_BINARY_DIR}/src/common ${K3D_BINARY_DIR}/src/vrml/vcom)

add_executable(makeDip makeDip.cpp dip_pin.cpp dip_case.cpp dip.cpp)

target_link_libraries(makeDip vcom)

install(TARGETS makeDip RUNTIME DESTINATION bin)