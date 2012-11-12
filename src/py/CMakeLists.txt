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
find_package(Boost 1.49 EXACT REQUIRED COMPONENTS python)

# The Python include directory must be specified
INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}" "${PYTHON_INCLUDE}")

include_directories(
    "${K3D_SOURCE_DIR}/include/common"
    "${K3D_SOURCE_DIR}/include/vrml/common"
    "${K3D_SOURCE_DIR}/include/vrml/models/conn"
    "${K3D_SOURCE_DIR}/pyinc/common"
    "${K3D_SOURCE_DIR}/pyinc/vrml/common"
    "${K3D_SOURCE_DIR}/pyinc/vrml/models/conn"
    )
    
link_directories(
	"${K3D_BINARY_DIR}/src/vrml/vcom"
	"${K3D_BINARY_DIR}/src/vrml/models/conn"
	)

add_library(kc3d SHARED kc3d.cpp)

target_link_libraries(kc3d vcom ${Boost_LIBRARIES} ${PYTHON_VERSION})

add_library(kc3dconn SHARED kc3dconn.cpp)

target_link_libraries(kc3dconn vconn ${Boost_LIBRARIES} ${PYTHON_VERSION})

# INSTALLATION
install(TARGETS kc3d LIBRARY DESTINATION lib)
install(TARGETS kc3dconn LIBRARY DESTINATION lib)
