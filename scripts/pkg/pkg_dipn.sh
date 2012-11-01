# This script packages DIP-n models
mkdir -p packages

tar cjvf packages/model_thru_dipn_`date -u +"%Y%m%d"`.tar.bz2 ../../mcad/vrml/thru/dil

