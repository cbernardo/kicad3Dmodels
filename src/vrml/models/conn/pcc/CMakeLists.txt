# file: ${K3D_SOURCE_DIR}/src/vrml/models/conn/pcc/CMakeLists.txt
#
# Model PCC-SMP-X horizontal mount thermocouple connector
#

include_directories(
    "${PROJECT_SOURCE_DIR}/include"
    "${K3D_SOURCE_DIR}/include/common"
    "${K3D_SOURCE_DIR}/include/vrml/common"
    )

link_directories(
    "${K3D_BINARY_DIR}/src/common"
    "${K3D_BINARY_DIR}/src/mat"
    "${K3D_BINARY_DIR}/src/vrml/vcom"
    )

add_executable( makePCC-SMP pccsmpx.cpp )

target_link_libraries(
    makePCC-SMP
    ${wxWidgets_LIBRARIES}
    kc3d_vcom
    )

install(
    TARGETS makePCC-SMP
    RUNTIME DESTINATION bin
    )
