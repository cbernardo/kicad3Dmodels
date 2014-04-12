# file: ${K3D_SOURCE_DIR}/src/py/CMakeLists.txt
#
# Python bindings for K3D tools
#

# This path must point to the boost library installation
SET(ENV{BOOST_ROOT} "/usr/lib")

# Set this to the Python include directory
SET(PYTHON_INCLUDE "/usr/include/python2.7")

# Set this to the python version to use
SET(PYTHON_VERSION python2.7)

# Ensure the specified Boost version matches what's installed
find_package(Boost 1.49 REQUIRED COMPONENTS python)

# The Python include directory must be specified
INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}" "${PYTHON_INCLUDE}")

include_directories(
    "${K3D_SOURCE_DIR}/include/common"
    "${K3D_SOURCE_DIR}/include/vrml/common"
    "${K3D_SOURCE_DIR}/include/vrml/models/conn"
    "${K3D_SOURCE_DIR}/include/vrml/models/resistor"
    "${K3D_SOURCE_DIR}/include/vrml/models/dil"
    "${K3D_SOURCE_DIR}/include/vrml/models/diode"
    "${K3D_SOURCE_DIR}/pyinc/common"
    "${K3D_SOURCE_DIR}/pyinc/vrml/common"
    "${K3D_SOURCE_DIR}/pyinc/vrml/models/conn"
    "${K3D_SOURCE_DIR}/pyinc/vrml/models/resistor"
    "${K3D_SOURCE_DIR}/pyinc/vrml/models/dil"
    "${K3D_SOURCE_DIR}/pyinc/vrml/models/diode"
    )
    
link_directories(
	"${K3D_BINARY_DIR}/src/vrml/vcom"
	"${K3D_BINARY_DIR}/src/vrml/models/conn"
    "${K3D_BINARY_DIR}/src/vrml/models/diode"
	)

# KC3D: General Tools
add_library(kc3d SHARED kc3d.cpp)
target_link_libraries(kc3d kc3d_vcom ${Boost_LIBRARIES} ${PYTHON_VERSION})

# KC3DCONN: Connectors
add_library(kc3dconn SHARED kc3dconn.cpp)
target_link_libraries(kc3dconn kc3d_vconn ${Boost_LIBRARIES} ${PYTHON_VERSION})

# KC3DRES: Resistors
add_library(kc3dres SHARED kc3dres.cpp)
target_link_libraries(kc3dres kc3d_vresist ${Boost_LIBRARIES} ${PYTHON_VERSION})

# KC3DDIP: DIL Packages
add_library(kc3ddip SHARED kc3ddip.cpp)
target_link_libraries(kc3ddip kc3d_vdip ${Boost_LIBRARIES} ${PYTHON_VERSION})

# KC3DDIP: DIL Packages
add_library(kc3ddiode SHARED kc3ddiode.cpp)
target_link_libraries(kc3ddiode kc3d_vdiode ${Boost_LIBRARIES} ${PYTHON_VERSION})

# INSTALLATION
install(TARGETS kc3d LIBRARY DESTINATION lib)
install(TARGETS kc3dconn LIBRARY DESTINATION lib)
install(TARGETS kc3dres LIBRARY DESTINATION lib)
install(TARGETS kc3ddip LIBRARY DESTINATION lib)
install(TARGETS kc3ddiode LIBRARY DESTINATION lib)

if( CMAKE_HOST_UNIX )

    add_custom_command(
        TARGET kc3d
        POST_BUILD
        COMMAND ln -s libkc3d.so kc3d.so
    )

    add_custom_command(
        TARGET kc3dconn
        POST_BUILD
        COMMAND ln -s libkc3dconn.so kc3dconn.so
    )

    add_custom_command(
        TARGET kc3ddiode
        POST_BUILD
        COMMAND ln -s libkc3ddiode.so kc3ddiode.so
    )

    add_custom_command(
        TARGET kc3ddip
        POST_BUILD
        COMMAND ln -s libkc3ddip.so kc3ddip.so
    )

    add_custom_command(
        TARGET kc3dres
        POST_BUILD
        COMMAND ln -s libkc3dres.so kc3dres.so
    )

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/kc3d.so"
    "${CMAKE_CURRENT_BINARY_DIR}/kc3dconn.so"
    "${CMAKE_CURRENT_BINARY_DIR}/kc3ddiode.so"
    "${CMAKE_CURRENT_BINARY_DIR}/kc3ddip.so"
    "${CMAKE_CURRENT_BINARY_DIR}/kc3dres.so"
    DESTINATION lib)

endif( CMAKE_HOST_UNIX )
