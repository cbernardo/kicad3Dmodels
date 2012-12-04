# Generate DO35 glass axial diodes
python ../../py/do35.py

IDIR="../../../mcad/vrml/thru/diode/horiz/do35"
mkdir -p ${IDIR}
mv -v do35*H*.wrl ${IDIR}

IDIR="../../../mcad/vrml/thru/diode/vert/do35"
mkdir -p ${IDIR}
mv -v do35*V*.wrl ${IDIR}
