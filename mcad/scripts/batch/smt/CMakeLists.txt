install(
    PROGRAMS
        generate.sh
    DESTINATION "${K3D_DATA}/scripts/batch/smt"
    )

install(
    FILES
        params.qfn
        params.qfp
        params.soic
        params.tssop
    DESTINATION "${K3D_DATA}/scripts/batch/smt"
    )
