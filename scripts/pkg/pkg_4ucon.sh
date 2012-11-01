# This script packages thru-connector models (currently only 4UCon Dwg 19950)
mkdir -p packages

tar cjvf packages/model_thru_conn_`date -u +"%Y%m%d"`.tar.bz2 ../../mcad/vrml/thru/conn
