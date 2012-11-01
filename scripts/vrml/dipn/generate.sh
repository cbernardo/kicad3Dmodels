# Generate DIPn models
../../bin/makeDip

IDIR="../../../mcad/vrml/thru/dil"

mkdir -p ${IDIR}
mv *.wrl ${IDIR}
