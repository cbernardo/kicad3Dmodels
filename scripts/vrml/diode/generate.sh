# Generate DO35 glass axial diodes
python ../../py/do35.py

IDIR="../../../mcad/vrml/thru/diode/horiz/do35"
mkdir -p ${IDIR}
mv -v do35*H*.wrl ${IDIR}

IDIR="../../../mcad/vrml/thru/diode/vert/do35"
mkdir -p ${IDIR}
mv -v do35*V*.wrl ${IDIR}

# Generate DO41 axial diodes (1N4001)
python ../../py/do41.py

IDIR="../../../mcad/vrml/thru/diode/horiz/do41"
mkdir -p ${IDIR}
mv -v do41*H*.wrl ${IDIR}

IDIR="../../../mcad/vrml/thru/diode/vert/do41"
mkdir -p ${IDIR}
mv -v do41*V*.wrl ${IDIR}

# Generate D201 axial diodes (1N5820)
python ../../py/do201.py

IDIR="../../../mcad/vrml/thru/diode/horiz/do201"
mkdir -p ${IDIR}
mv -v do201*H*.wrl ${IDIR}

IDIR="../../../mcad/vrml/thru/diode/vert/do201"
mkdir -p ${IDIR}
mv -v do201*V*.wrl ${IDIR}

# Generate D204 axial diodes (ex: IR80SQ035)
python ../../py/do204.py

IDIR="../../../mcad/vrml/thru/diode/horiz/do204"
mkdir -p ${IDIR}
mv -v do204*H*.wrl ${IDIR}

IDIR="../../../mcad/vrml/thru/diode/vert/do204"
mkdir -p ${IDIR}
mv -v do204*V*.wrl ${IDIR}
