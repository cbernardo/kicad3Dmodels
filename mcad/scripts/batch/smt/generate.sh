SDIR="$(realpath "$(dirname "$0")")"
PDIR="$(realpath "$(dirname "$0")/../../py")"

# QFP devices
python ${PDIR}/smt_qfp_soic.py ${SDIR}/params.qfp

IDIR="mcad/vrml/smt/lqfp"
mkdir -p ${IDIR}
mv LQFP*.wrl ${IDIR}

IDIR="mcad/vrml/smt/pqfp"
mkdir -p ${IDIR}
mv PQFP*.wrl ${IDIR}

IDIR="mcad/vrml/smt/tqfp"
mkdir -p ${IDIR}
mv TQFP*.wrl ${IDIR}

# SOIC devices
python ${PDIR}/smt_qfp_soic.py ${SDIR}/params.soic

IDIR="mcad/vrml/smt/soic"
mkdir -p ${IDIR}
mv SOI*.wrl ${IDIR}

# TSSOP devices
python ${PDIR}/smt_qfp_soic.py ${SDIR}/params.tssop

IDIR="mcad/vrml/smt/msop"
mkdir -p ${IDIR}
mv MSOP*.wrl ${IDIR}

IDIR="mcad/vrml/smt/qsop"
mkdir -p ${IDIR}
mv QSOP*.wrl ${IDIR}

IDIR="mcad/vrml/smt/ssop"
mkdir -p ${IDIR}
mv SSOP*.wrl ${IDIR}

IDIR="mcad/vrml/smt/tssop"
mkdir -p ${IDIR}
mv TSSOP*.wrl ${IDIR}

IDIR="mcad/vrml/smt/vso"
mkdir -p ${IDIR}
mv VSO*.wrl ${IDIR}

# DFN/QFN devices
python ${PDIR}/smt_qfp_soic.py ${SDIR}/params.qfn

IDIR="mcad/vrml/smt/dfn"
mkdir -p ${IDIR}
mv DFN*.wrl ${IDIR}
mv ?DFN*.wrl ${IDIR}

IDIR="mcad/vrml/smt/qfn"
mkdir -p ${IDIR}
mv QFN*.wrl ${IDIR}
mv ?QFN*.wrl ${IDIR}
