# This script packages thru-connector models for Samtec series SS, ESS, HSS, SD, ESD
mkdir -p packages

tar cjvf packages/model_thru_con_sam_sl_`date -u +"%Y%m%d"`.tar.bz2 ../../mcad/vrml/thru/conn/samtec/sl ../../mcad/vrml/thru/conn/samtec/sld
