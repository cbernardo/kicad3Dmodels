# Project: KiCAD3DModels (K3D)
#

# NOTE: In the future, the selection of files to compile will
# depend on the type of model being built (VRML or Free-CAD)

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project (K3D CXX)

# The top level directory is ${K3D_SOURCE_DIR}
# The top level build directory is ${K3D_BINARY_DIR}

# NOTE: we should always compile with -Wall, but the -g
# option should be controlled by a 'DEBUG' switch.
# These switches are also specific to gcc so this control
# file is not portable across compilers.
set(CMAKE_CXX_FLAGS "-g -Wall -DBOOST_PYTHON_MAX_ARITY=20")

# install executables in the scripting directory
# set(CMAKE_INSTALL_PREFIX ${K3D_SOURCE_DIR}/scripts)

# Version information
set(K3D_VERSION_MAJOR 0)
set(K3D_VERSION_MINOR 10)
#set(K3D_TOP_DIR ${K3D_SOURCE_DIR})
set(K3D_TOP_DIR "${CMAKE_INSTALL_PREFIX}/share")

# create a config file
configure_file(${K3D_SOURCE_DIR}/src/k3dconfig.h.in ${K3D_BINARY_DIR}/src/k3dconfig.h)
include_directories(${K3D_BINARY_DIR}/src)


# pull in the sources
add_subdirectory (src)

# As the project develops we will want to do the following:
#
# + build the executables
# + install the executables to a fixed subdirectory
# + run scripts from a subdirectory; the scripts run the
#	executables and move the output to their places
#	in the model directory tree
#

# install the material appearance definitions
install(
    DIRECTORY mcad/colors/
    DESTINATION "${CMAKE_INSTALL_PREFIX}/share/mcad/colors"
)
