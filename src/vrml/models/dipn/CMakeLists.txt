# file: ${K3D_SOURCE_DIR}/src/vrml/models/dipn/CMakeLists.txt
#
# Parametric model for DIPn packages
#

include_directories(
    "${PROJECT_SOURCE_DIR}/include"
    "${K3D_SOURCE_DIR}/include/common"
    "${K3D_SOURCE_DIR}/include/vrml/common"
    "${K3D_SOURCE_DIR}/include/vrml/models/dil"
    )

link_directories(
    "${K3D_BINARY_DIR}/src/common"
    "${K3D_BINARY_DIR}/src/vrml/vcom"
    )

add_executable( makeDip
    makeDip.cpp
    dip_pin.cpp
    dip_case.cpp
    dip.cpp
    )

target_link_libraries(
    makeDip
    ${wxWidgets_LIBRARIES}
    kc3d_vcom
    )

add_library(
    kc3d_vdip SHARED
    dip.cpp
    dip_pin.cpp
    dip_case.cpp
    )

set_target_properties( kc3d_vdip PROPERTIES PREFIX "" )

target_link_libraries(
    kc3d_vdip
    "-Wl,-Bdynamic" kc3d_vcom
    )

install(
    TARGETS makeDip
    RUNTIME DESTINATION bin
    )

install(
    TARGETS kc3d_vdip
    ${K3D_LIBDIR}
    )
