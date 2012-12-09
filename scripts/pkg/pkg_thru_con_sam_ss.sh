# This script packages thru-connector models for Samtec series SS, ESS, HSS, SD, ESD
mkdir -p packages

tar cjvf packages/model_thru_con_sam_ss_`date -u +"%Y%m%d"`.tar.bz2 ../../mcad/vrml/thru/conn/samtec/ss ../../mcad/vrml/thru/conn/samtec/ess ../../mcad/vrml/thru/conn/samtec/hss ../../mcad/vrml/thru/conn/samtec/sd ../../mcad/vrml/thru/conn/samtec/esd
