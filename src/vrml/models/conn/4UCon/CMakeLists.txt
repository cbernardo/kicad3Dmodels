# file: ${K3D_SOURCE_DIR}/src/vrml/4UCon/CMakeLists.txt
#
# Parametric model for 4UCon SIL header packages, Drawing 19950
#

include_directories(${K3D_SOURCE_DIR}/include/common ${K3D_SOURCE_DIR}/include/vrml/common) 

link_directories(${K3D_BINARY_DIR}/src/common ${K3D_BINARY_DIR}/src/vrml/vcom)

add_executable(make19950 make19950.cpp)

target_link_libraries(make19950 kc3d_vcom)

install(TARGETS make19950 RUNTIME DESTINATION bin)
