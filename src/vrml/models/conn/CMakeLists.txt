# file: ${K3D_SOURCE_DIR}/src/vrml/models/conn/CMakeLists.txt
#
# Top level directory for connector models
#

# 4UCon connectors
add_subdirectory (4UCon)

# PCC-SMP/OST Thermocouple connectors
add_subdirectory (pcc)

# Generic PCB headers
add_subdirectory (hdr)

# Sections above are for initial test and
# concept demonstrator code. Although they
# shall be retained as stand-alone code,
# the code should be refactored and branched
# so that they are Python friendly.

# Sections below are new Python friendly code
# organized into a k3dconn library for
# generating connector 3D models from
# existing parametric models.

include_directories(
    "${K3D_SOURCE_DIR}/include/common"
    "${K3D_SOURCE_DIR}/include/vrml/common"
    "${K3D_SOURCE_DIR}/include/vrml/models/conn"
    )
    
add_library(vconn SHARED
    hdr/genhdr.cpp 
    )

#    include_directories(
#    "${K3D_SOURCE_DIR}/include/common"
#    "${K3D_SOURCE_DIR}/include/vrml/common"
#    ) 


target_link_libraries(vconn vcom)

install(TARGETS vconn LIBRARY DESTINATION lib)
    
