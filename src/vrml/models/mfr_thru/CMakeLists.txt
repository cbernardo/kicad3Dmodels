# file: ${K3D_SOURCE_DIR}/src/vrml/models/mfr_thru/CMakeLists.txt
#

include_directories(
    "${K3D_SOURCE_DIR}/include/common"
    "${K3D_SOURCE_DIR}/include/vrml/common"
    "${K3D_SOURCE_DIR}/include/vrml/models/resistor"
    )

link_directories(${K3D_BINARY_DIR}/src/common ${K3D_BINARY_DIR}/src/vrml/vcom)

# Tool for generating E192, E96, E48, E24, or E12 series ouput
# specification files for use by makeMFR
add_executable(makeEnnn makeEnnn.cpp colorcodes.cpp)

# Parametric model for tubular thru-hole resistors
add_executable(makeMFR makeMFR.cpp resistor.cpp)
target_link_libraries(makeMFR vcom)

install(TARGETS makeMFR makeEnnn RUNTIME DESTINATION bin)


# Sections above are for initial test and
# concept demonstrator code. Although they
# shall be retained as stand-alone code,
# the code should be refactored and branched
# so that they are Python friendly.

# Sections below are new Python friendly code
# organized into a k3dresistor library for
# generating resistor 3D models from
# existing parametric models.

include_directories(
    "${K3D_SOURCE_DIR}/include/common"
    "${K3D_SOURCE_DIR}/include/vrml/common"
    "${K3D_SOURCE_DIR}/include/vrml/models/resistor"
    )
    
add_library(vresist SHARED
    resistor.cpp 
    )

#    include_directories(
#    "${K3D_SOURCE_DIR}/include/common"
#    "${K3D_SOURCE_DIR}/include/vrml/common"
#    ) 


target_link_libraries(vresist vcom)

install(TARGETS vresist LIBRARY DESTINATION lib)