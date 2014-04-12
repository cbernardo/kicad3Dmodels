# file: ${K3D_SOURCE_DIR}/src/vrml/pcc/CMakeLists.txt
#
# Model PCC-SMP-X horizontal mount thermocouple connector
#
set(CMAKE_CXX_FLAGS "-g -Wall")

include_directories(${K3D_SOURCE_DIR}/include/common ${K3D_SOURCE_DIR}/include/vrml/common) 

link_directories(${K3D_BINARY_DIR}/src/vrml/vcom)

add_executable(vrmlstitch vrmlstitch.cpp)

target_link_libraries(vrmlstitch kc3d_vcom)

install(TARGETS vrmlstitch RUNTIME DESTINATION bin)
