# Project: KiCAD3DModels (K3D)
#
project( K3D CXX C )

cmake_minimum_required( VERSION 2.8.4 FATAL_ERROR )

# Version information
set( K3D_VERSION_MAJOR 0 )
set( K3D_VERSION_MINOR 12 )

find_package( wxWidgets 2.8.0 COMPONENTS core base REQUIRED )

if( NOT wxWidgets_FOUND )
    message( FATAL_ERROR "wxWidgets >= 2.8 is required (not found)." )
endif()

include( ${wxWidgets_USE_FILE} )

find_package( OpenGL QUIET )

if( NOT OPENGL_FOUND )
    message( FATAL_ERROR "OpenGL is required (not found)." )
endif()

find_package( Boost 1.54.0 REQUIRED COMPONENTS python )

if( NOT Boost_FOUND )
    message( FATAL_ERROR "Boost >=1.54 is required (not found)." )
endif()

add_custom_target( boost )

set( PythonInterp_FIND_VERSION 2.6 )
set( PythonLibs_FIND_VERSION 2.6 )

find_package( PythonInterp )

if( NOT PYTHONINTERP_FOUND )
    message( FATAL_ERROR "No Python interpreter found." )
endif()

if( NOT PYTHON_VERSION_MAJOR EQUAL 2 )
    message( FATAL_ERROR "Python 2.x is required (not found)." )
endif()

find_package( PythonLibs 2.6 )

set( CMAKE_CXX_FLAGS "-Wall -DBOOST_PYTHON_MAX_ARITY=20" )

# The top level VRML tools directory is ${K3D_SOURCE_DIR}
set( K3D_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" )
# The top level VRML tools build directory is ${K3D_BINARY_DIR}
set( K3D_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}" )

set( K3D_DATA "${CMAKE_INSTALL_PREFIX}/share/kc3d" )
# NOTE: K3D_TOP_DIR is used only by 'hdrtest.cpp' which may eventually be removed
set( K3D_TOP_DIR "${CMAKE_INSTALL_PREFIX}/share" )

if( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
    set( K3D_LIBDIR RUNTIME DESTINATION bin )
else()
    set( K3D_LIBDIR LIBRARY DESTINATION lib/kc3d )
endif()

# create a config file
configure_file(
    "${K3D_SOURCE_DIR}/src/k3dconfig.h.in"
    "${K3D_BINARY_DIR}/src/k3dconfig.h"
    )

# ensure that the newly created k3dconfig.h can be found
include_directories( "${K3D_BINARY_DIR}/src" )

# pull in the sources
add_subdirectory ( src )

# As the project develops we will want to do the following:
#
# + build the executables
# + install the executables to a fixed subdirectory
# + run scripts from a subdirectory; the scripts run the
#   executables and move the output to their places
#   in the model directory tree
#

add_subdirectory ( mcad )
