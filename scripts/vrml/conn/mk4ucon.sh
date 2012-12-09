# Create the 4UCon Drawing 19950 SIL headers, 3-40 pins
../../bin/make19950

IDIR="../../../mcad/vrml/thru/conn/4UCON"

mkdir -p ${IDIR}
mv 4UCON*.wrl ${IDIR}
