# batch model generation scripts
add_subdirectory ( batch )

# demo python scripts
add_subdirectory ( demo )

# python models of actual components
add_subdirectory ( py )

install(
    FILES README
    DESTINATION "${K3D_DATA}/scripts"
    )
