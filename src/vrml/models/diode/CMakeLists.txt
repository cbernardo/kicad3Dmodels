# file: ${K3D_SOURCE_DIR}/src/vrml/models/diode/CMakeLists.txt
#
# Top level directory for diode models
#

include_directories(
    "${K3D_SOURCE_DIR}/include/common"
    "${K3D_SOURCE_DIR}/include/vrml/common"
    "${K3D_SOURCE_DIR}/include/vrml/models/diode"
    )

add_library(
    kc3d_vdiode SHARED
    do35/do35.cpp
    generic/gendiode.cpp
    )

set_target_properties( kc3d_vdiode PROPERTIES PREFIX "" )

target_link_libraries(
    kc3d_vdiode
    "-Wl,-Bdynamic" kc3d_vcom
    )

install(
    TARGETS kc3d_vdiode
    ${K3D_LIBDIR}
    )
