# file: ${K3D_SOURCE_DIR}/src/vrml/models/hdr/CMakeLists.txt
#
# Parametric model for generic header packages
#

include_directories(
    "${K3D_SOURCE_DIR}/include/common"
    "${K3D_SOURCE_DIR}/include/vrml/common"
    "${K3D_SOURCE_DIR}/include/vrml/models/conn"
    )

link_directories(
    "${K3D_BINARY_DIR}/src/common"
    "${K3D_BINARY_DIR}/src/vrml/vcom"
    "${K3D_BINARY_DIR}/src/vrml/models/conn"
    )

# DEPRECATED:
# Instructions for 'hdrtest' application. This is
# a test application which may have no further use
# and may be removed in the future

#add_executable(testhdr hdrtest.cpp)

#target_link_libraries(test vconn vcom)

#install(TARGETS testhdr RUNTIME DESTINATION bin)
