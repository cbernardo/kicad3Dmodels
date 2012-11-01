# This script packages vertical oriented thru-hole resistors
mkdir -p packages

tar cjvf packages/model_thru_resv_`date -u +"%Y%m%d"`.tar.bz2 ../../mcad/vrml/thru/res/vert

