# file: ${K3D_SOURCE_DIR}/src/mat/CMakeLists.txt
#

# NOTE: In the future the library will depend on whether we
#	are building VRML, FreeCAD, or ALL models

include_directories(${K3D_SOURCE_DIR}/src/common)

add_library(material material.cpp vrmlmat.cpp)

target_link_libraries(material common)