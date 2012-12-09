# This script packages thru-connector models (currently 4UCon Dwg 19950, PCC-SMP)
mkdir -p packages

tar cjvf packages/model_thru_conn_`date -u +"%Y%m%d"`.tar.bz2 ../../mcad/vrml/thru/conn/4UCON ../../mcad/vrml/thru/conn/PCC
