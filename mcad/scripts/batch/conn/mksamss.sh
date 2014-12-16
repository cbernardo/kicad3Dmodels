# Create the Samtec SS/ESS/HSS + SD/ESD series female headers

# locate the scripts directory
DIRPY="$(realpath "$(dirname "$0")/../../py")"

python "${DIRPY}/samtec_ss_sd_ess_esd_hss.py"

# SS
IDIR="mcad/vrml/thru/conn/samtec/ss/t02"
mkdir -p ${IDIR}
mv samtec_SS_1*T_2_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/ss/t38"
mkdir -p ${IDIR}
mv samtec_SS_1*T_38_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/ss/t07"
mkdir -p ${IDIR}
mv samtec_SS_1*T_7_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/ss/t5a"
mkdir -p ${IDIR}
mv samtec_SS_1*T_5A_*.wrl ${IDIR}

# HSS
IDIR="mcad/vrml/thru/conn/samtec/hss/t02"
mkdir -p ${IDIR}
mv samtec_HSS_1*T_2_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/hss/t38"
mkdir -p ${IDIR}
mv samtec_HSS_1*T_38_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/hss/t07"
mkdir -p ${IDIR}
mv samtec_HSS_1*T_7_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/hss/t5a"
mkdir -p ${IDIR}
mv samtec_HSS_1*T_5A_*.wrl ${IDIR}

# ESS
IDIR="mcad/vrml/thru/conn/samtec/ess/t03"
mkdir -p ${IDIR}
mv samtec_ESS_1*T_03_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/ess/t04"
mkdir -p ${IDIR}
mv samtec_ESS_1*T_04_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/ess/t05"
mkdir -p ${IDIR}
mv samtec_ESS_1*T_05_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/ess/t06"
mkdir -p ${IDIR}
mv samtec_ESS_1*T_06_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/ess/t07"
mkdir -p ${IDIR}
mv samtec_ESS_1*T_07_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/ess/t08"
mkdir -p ${IDIR}
mv samtec_ESS_1*T_08_*.wrl ${IDIR}

# SD
IDIR="mcad/vrml/thru/conn/samtec/sd/t02"
mkdir -p ${IDIR}
mv samtec_SD_1*T_2_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/sd/t38"
mkdir -p ${IDIR}
mv samtec_SD_1*T_38_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/sd/t07"
mkdir -p ${IDIR}
mv samtec_SD_1*T_7_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/sd/t5a"
mkdir -p ${IDIR}
mv samtec_SD_1*T_5A_*.wrl ${IDIR}

# ESD
IDIR="mcad/vrml/thru/conn/samtec/esd/t03"
mkdir -p ${IDIR}
mv samtec_ESD_1*T_03_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/esd/t04"
mkdir -p ${IDIR}
mv samtec_ESD_1*T_04_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/esd/t05"
mkdir -p ${IDIR}
mv samtec_ESD_1*T_05_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/esd/t06"
mkdir -p ${IDIR}
mv samtec_ESD_1*T_06_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/esd/t07"
mkdir -p ${IDIR}
mv samtec_ESD_1*T_07_*.wrl ${IDIR}

IDIR="mcad/vrml/thru/conn/samtec/esd/t08"
mkdir -p ${IDIR}
mv samtec_ESD_1*T_08_*.wrl ${IDIR}
