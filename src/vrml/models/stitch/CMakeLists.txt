# file: ${K3D_SOURCE_DIR}/src/vrml/models/stitch/CMakeLists.txt
#
# vrmlstitch tool for manual assembly of a VRML scene from existing models
#

set(CMAKE_CXX_FLAGS "-g -Wall")

include_directories(
    "${K3D_SOURCE_DIR}/include/common"
    "${K3D_SOURCE_DIR}/include/vrml/common"
    )

add_executable(
    vrmlstitch
    vrmlstitch.cpp
    )

target_link_libraries(
    vrmlstitch
    kc3d_vcom
    )

install(
    TARGETS vrmlstitch
    RUNTIME DESTINATION bin
    )
