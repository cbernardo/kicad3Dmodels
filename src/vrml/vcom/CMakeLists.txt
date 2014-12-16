# file: ${K3D_SOURCE_DIR}/src/vrml/vcom/CMakeLists.txt
#
# VRML common tools
#

include_directories(
    "${K3D_SOURCE_DIR}/include/common"
    "${K3D_SOURCE_DIR}/include/vrml/common"
    ${OPENGL_INCLUDE_DIR}
    )

link_directories( "${K3D_BINARY_DIR}/src/common" )

add_library(
    kc3d_vcom SHARED
    vcom.cpp
    polygon.cpp
    circle.cpp
    rectangle.cpp
    pin.cpp
    shoulder.cpp
    funnel.cpp
    hole.cpp
    hdrbase.cpp
    wire.cpp
    dimple.cpp
    kc3dtess.cpp
    )

set_target_properties( kc3d_vcom PROPERTIES PREFIX "" )

target_link_libraries(
    kc3d_vcom
    "-Wl,-Bdynamic" kc3d_common
    "-Wl,-Bdynamic" ${OPENGL_LIBRARIES}
    "-Wl,-Bdynamic" ${wxWidgets_LIBRARIES}
    )

install(
    TARGETS kc3d_vcom
    ${K3D_LIBDIR}
    )
