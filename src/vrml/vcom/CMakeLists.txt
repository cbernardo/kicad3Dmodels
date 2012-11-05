# file: ${K3D_SOURCE_DIR}/src/vrml/vcom/CMakeLists.txt
#
# VRML common tools
#

include_directories(${K3D_SOURCE_DIR}/src/common) 

add_library(vcom vcom.cpp polygon.cpp circle.cpp rectangle.cpp pin.cpp shoulder.cpp funnel.cpp hole.cpp)

target_link_libraries(vcom common)
