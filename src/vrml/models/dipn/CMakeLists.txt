# file: ${K3D_SOURCE_DIR}/src/vrml/models/dipn/CMakeLists.txt
#
# Parametric model for DIPn packages
#

include_directories(
    "${K3D_SOURCE_DIR}/include/common"
    "${K3D_SOURCE_DIR}/include/vrml/common"
    "${K3D_SOURCE_DIR}/include/vrml/models/dil"
    ) 

link_directories(${K3D_BINARY_DIR}/src/common ${K3D_BINARY_DIR}/src/vrml/vcom)

add_executable(makeDip makeDip.cpp dip_pin.cpp dip_case.cpp dip.cpp)

target_link_libraries(makeDip kc3d_vcom)

add_library(kc3d_vdip SHARED
    dip.cpp dip_pin.cpp dip_case.cpp
    )
target_link_libraries(kc3d_vdip kc3d_vcom)

install(TARGETS makeDip RUNTIME DESTINATION bin)

install(TARGETS kc3d_vdip LIBRARY DESTINATION lib)
