# file: ${K3D_SOURCE_DIR}/src/vrml/models/diode/CMakeLists.txt
#
# Top level directory for diode models
#

include_directories(
    "${K3D_SOURCE_DIR}/include/common"
    "${K3D_SOURCE_DIR}/include/vrml/common"
    "${K3D_SOURCE_DIR}/include/vrml/models/diode"
    )
    
add_library(vdiode
    do35/do35.cpp 
    )

target_link_libraries(vdiode vcom)
