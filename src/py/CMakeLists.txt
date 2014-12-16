# file: ${K3D_SOURCE_DIR}/src/py/CMakeLists.txt
#
# Python bindings for K3D tools
#

include_directories(
    "${Boost_INCLUDE_DIR}"
    "${PYTHON_INCLUDE_DIR}"
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
    "${Boost_LIBRARY_DIR}"
    "${K3D_BINARY_DIR}/src/vrml/vcom"
    "${K3D_BINARY_DIR}/src/vrml/models/conn"
    "${K3D_BINARY_DIR}/src/vrml/models/diode"
    )

# NOTE: on Linux a static link to boost-python
# results in broken tools; this is a bug waiting
# to be fixed. It is not known if other platforms
# would be affected.
if( APPLE )
    set( BOOST_LINK_FLAGS "-Wl,-Bstatic" )
else()
    set( BOOST_LINK_FLAGS "-Wl,-Bdynamic" )
endif()

# KC3D: General Tools
add_library( kc3d SHARED kc3d.cpp )

set_target_properties( kc3d PROPERTIES PREFIX "" )

if( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
    set_target_properties( kc3d PROPERTIES SUFFIX ".pyd" )
endif()

add_dependencies( kc3d boost )

target_link_libraries(
    kc3d
    "${BOOST_LINK_FLAGS}" "${Boost_PYTHON_LIBRARY}"
    "-Wl,-Bdynamic" kc3d_vcom
    "-Wl,-Bdynamic" ${PYTHON_LIBRARY}
    )

# KC3DCONN: Connectors
add_library( kc3dconn SHARED kc3dconn.cpp )

set_target_properties( kc3dconn PROPERTIES PREFIX "" )

if( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
    set_target_properties( kc3dconn PROPERTIES SUFFIX ".pyd" )
endif()

add_dependencies( kc3dconn boost  )

target_link_libraries(
    kc3dconn
    ${BOOST_LINK_FLAGS} "${Boost_PYTHON_LIBRARY}"
    "-Wl,-Bdynamic" kc3d_vconn
    "-Wl,-Bdynamic" ${PYTHON_LIBRARY}
    )

# KC3DRES: Resistors
add_library( kc3dres SHARED kc3dres.cpp )

set_target_properties( kc3dres PROPERTIES PREFIX "" )

if( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
    set_target_properties( kc3dres PROPERTIES SUFFIX ".pyd" )
endif()

add_dependencies( kc3dres boost  )

target_link_libraries(
    kc3dres
    ${BOOST_LINK_FLAGS} "${Boost_PYTHON_LIBRARY}"
    "-Wl,-Bdynamic" kc3d_vresist
    "-Wl,-Bdynamic" ${PYTHON_LIBRARY}
    )

# KC3DDIP: DIL Packages
add_library(kc3ddip SHARED kc3ddip.cpp)

set_target_properties( kc3ddip PROPERTIES PREFIX "" )

if( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
    set_target_properties( kc3ddip PROPERTIES SUFFIX ".pyd" )
endif()

add_dependencies( kc3ddip boost  )

target_link_libraries(
    kc3ddip
    ${BOOST_LINK_FLAGS} "${Boost_PYTHON_LIBRARY}"
    "-Wl,-Bdynamic" kc3d_vdip
    "-Wl,-Bdynamic" ${PYTHON_LIBRARY}
    )

# KC3DDIP: DIL Packages
add_library( kc3ddiode SHARED kc3ddiode.cpp )

set_target_properties( kc3ddiode PROPERTIES PREFIX "" )

if( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
    set_target_properties( kc3ddiode PROPERTIES SUFFIX ".pyd" )
endif()

add_dependencies( kc3ddiode boost  )

target_link_libraries(
    kc3ddiode
    ${BOOST_LINK_FLAGS} "${Boost_PYTHON_LIBRARY}"
    "-Wl,-Bdynamic" kc3d_vdiode
    "-Wl,-Bdynamic" ${PYTHON_LIBRARY}
    )

# Installation
install(
    TARGETS
        kc3d
        kc3dconn
        kc3dres
        kc3ddip
        kc3ddiode
    ${K3D_LIBDIR}
    )
