# This script packages thru-connector models for Samtec series TSW
mkdir -p packages

tar cjvf packages/model_thru_con_sam_tsw_`date -u +"%Y%m%d"`.tar.bz2 ../../mcad/vrml/thru/conn/samtec/tsw 
