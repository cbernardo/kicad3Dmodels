# file: ${K3D_SOURCE_DIR}/src/vrml/models/conn/CMakeLists.txt
#
# Top level directory for connector models
#

# Binary executables; these are not Python scriptable
add_subdirectory( 4UCon )

# PCC-SMP/OST Thermocouple connectors
add_subdirectory( pcc )

# Generic PCB headers
add_subdirectory( hdr )

# Python scriptable code organized into a k3dconn library for
# generating connector 3D models from existing parametric models.

include_directories(
    "${K3D_SOURCE_DIR}/include/common"
    "${K3D_SOURCE_DIR}/include/vrml/common"
    "${K3D_SOURCE_DIR}/include/vrml/models/conn"
    )

add_library(
    kc3d_vconn SHARED
    hdr/genhdr.cpp
    )

set_target_properties( kc3d_vconn PROPERTIES PREFIX "" )

target_link_libraries(
    kc3d_vconn
    "-Wl,-Bdynamic" kc3d_vcom
    )

install(
    TARGETS kc3d_vconn
    ${K3D_LIBDIR}
    )
