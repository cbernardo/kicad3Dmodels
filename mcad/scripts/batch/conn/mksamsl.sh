# Create the Samtec SL, SLD series female headers

# locate the scripts directory
DIRPY="$(realpath "$(dirname "$0")/../../py")"

python "${DIRPY}/samtec_sl_sld.py"

# SL
IDIR="mcad/vrml/thru/conn/samtec/sl/t10"
mkdir -p ${IDIR}
mv samtec_SL_1*T_10.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/sl/t11"
mkdir -p ${IDIR}
mv samtec_SL_1*T_11.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/sl/t12"
mkdir -p ${IDIR}
mv samtec_SL_1*T_12.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/sl/t19"
mkdir -p ${IDIR}
mv samtec_SL_1*T_19.wrl ${IDIR}

# SLD
IDIR="mcad/vrml/thru/conn/samtec/sld/t10"
mkdir -p ${IDIR}
mv samtec_SLD_1*T_10.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/sld/t11"
mkdir -p ${IDIR}
mv samtec_SLD_1*T_11.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/sld/t12"
mkdir -p ${IDIR}
mv samtec_SLD_1*T_12.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/sld/t19"
mkdir -p ${IDIR}
mv samtec_SLD_1*T_19.wrl ${IDIR}
