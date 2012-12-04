# This script packages the thru-hole diode models
mkdir -p packages

tar cjvf packages/model_thru_diode_`date -u +"%Y%m%d"`.tar.bz2 ../../mcad/vrml/thru/diode
