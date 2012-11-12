# file: ${K3D_SOURCE_DIR}/src/vrml/tools/CMakeLists.txt
#
# Top level directory for VRML specific sources
#

# DIPn models
add_subdirectory (dipn)

# Thru-hole resistors
add_subdirectory (mfr_thru)

# Connectors
add_subdirectory (conn)

# VRML stitching and assembly tools
add_subdirectory (stitch)
