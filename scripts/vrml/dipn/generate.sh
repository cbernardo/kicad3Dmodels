# Generate DIPn models
../../bin/makeDip

IDIR="../../../mcad/vrml/thru/dil/I300"
mkdir -p ${IDIR}
mv *i300.wrl ${IDIR}

IDIR="../../../mcad/vrml/thru/dil/I600"
mkdir -p ${IDIR}
mv *i600.wrl ${IDIR}
