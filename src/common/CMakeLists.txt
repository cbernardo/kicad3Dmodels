# file: ${K3D_SOURCE_DIR}/src/common/CMakeLists.txt
#

# NOTE: In the future the library will depend on whether we
#	are building VRML, FreeCAD, or ALL models

include_directories(${K3D_SOURCE_DIR}/include/common)

add_library(common SHARED keyvalparser.cpp transform.cpp material.cpp vrmlmat.cpp)
