install(
    PROGRAMS
        generate.sh
    DESTINATION "${K3D_DATA}/scripts/batch/resistor"
    )

install(
    FILES
        README
    DESTINATION "${K3D_DATA}/scripts/batch/resistor"
    )

install(
    DIRECTORY params/
    DESTINATION "${K3D_DATA}/scripts/batch/resistor/params"
    )

install(
    DIRECTORY stitch/
    DESTINATION "${K3D_DATA}/scripts/batch/resistor/stitch"
    )
