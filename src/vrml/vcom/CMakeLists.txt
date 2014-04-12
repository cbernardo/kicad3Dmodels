# file: ${K3D_SOURCE_DIR}/src/vrml/vcom/CMakeLists.txt
#
# VRML common tools
#

include_directories(
    "${K3D_SOURCE_DIR}/include/common"
    "${K3D_SOURCE_DIR}/include/vrml/common"
    ) 

add_library(kc3d_vcom SHARED vcom.cpp polygon.cpp circle.cpp rectangle.cpp
    pin.cpp shoulder.cpp funnel.cpp hole.cpp hdrbase.cpp
    wire.cpp)

target_link_libraries(kc3d_vcom kc3d_common)

install(TARGETS kc3d_vcom LIBRARY DESTINATION lib)
