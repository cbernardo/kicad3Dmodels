# Generic E96_E24, 1% MFR, 0.5W 040 lead spacing, horizontal
../../bin/makeMFR -p params/genericMFR_0W50_040h -o out_lists/Generic_E96_1.list
../../bin/makeMFR -p params/genericMFR_0W50_040h -o out_lists/Generic_E24_1.list

IDIR="../../../mcad/vrml/thru/res/horiz/MFR_1/0W50/0I40"

mkdir -p ${IDIR}/1R
mv MFR_?R*wrl ${IDIR}/1R

mkdir -p ${IDIR}/10R
mv MFR_??R*wrl ${IDIR}/10R

mkdir -p ${IDIR}/100R
mv MFR_???R*wrl ${IDIR}/100R

mkdir -p ${IDIR}/1K
mv MFR_?K*wrl ${IDIR}/1K

mkdir -p ${IDIR}/10K
mv MFR_??K*wrl ${IDIR}/10K

mkdir -p ${IDIR}/100K
mv MFR_???K*wrl ${IDIR}/100K

mkdir -p ${IDIR}/1M
mv MFR_?M*wrl ${IDIR}/1M

mkdir -p ${IDIR}/10M
mv MFR_??M*wrl ${IDIR}/10M


# Generic E48, 2% MFR, 0.5W 040 lead spacing, horizontal
../../bin/makeMFR -p params/genericMFR_0W50_040h -o out_lists/Generic_E48_2.list

IDIR="../../../mcad/vrml/thru/res/horiz/MFR_2/0W50/0I40"

mkdir -p ${IDIR}/1R
mv MFR_?R*wrl ${IDIR}/1R

mkdir -p ${IDIR}/10R
mv MFR_??R*wrl ${IDIR}/10R

mkdir -p ${IDIR}/100R
mv MFR_???R*wrl ${IDIR}/100R

mkdir -p ${IDIR}/1K
mv MFR_?K*wrl ${IDIR}/1K

mkdir -p ${IDIR}/10K
mv MFR_??K*wrl ${IDIR}/10K

mkdir -p ${IDIR}/100K
mv MFR_???K*wrl ${IDIR}/100K

mkdir -p ${IDIR}/1M
mv MFR_?M*wrl ${IDIR}/1M

mkdir -p ${IDIR}/10M
mv MFR_??M*wrl ${IDIR}/10M


# Generic E24, 5% CCR, 0.25W 040 lead spacing, horizontal
../../bin/makeMFR -p params/genericCCR_0W25_040h -o out_lists/Generic_E24_5.list

IDIR="../../../mcad/vrml/thru/res/horiz/CCR_5/0W25/0I40"

mkdir -p ${IDIR}/1R
mv CCR_?R*wrl ${IDIR}/1R

mkdir -p ${IDIR}/10R
mv CCR_??R*wrl ${IDIR}/10R

mkdir -p ${IDIR}/100R
mv CCR_???R*wrl ${IDIR}/100R

mkdir -p ${IDIR}/1K
mv CCR_?K*wrl ${IDIR}/1K

mkdir -p ${IDIR}/10K
mv CCR_??K*wrl ${IDIR}/10K

mkdir -p ${IDIR}/100K
mv CCR_???K*wrl ${IDIR}/100K

mkdir -p ${IDIR}/1M
mv CCR_?M*wrl ${IDIR}/1M

mkdir -p ${IDIR}/10M
mv CCR_??M*wrl ${IDIR}/10M

mkdir -p ${IDIR}/100M
mv CCR_???M*wrl ${IDIR}/100M

mkdir -p ${IDIR}/1G
mv CCR_?G*wrl ${IDIR}/1G


# Generic E96_E24, 1% MFR, 0.5W 040 lead spacing, vertical
../../bin/makeMFR -p params/genericMFR_0W50_040v -o out_lists/Generic_E96_1.list
../../bin/makeMFR -p params/genericMFR_0W50_040v -o out_lists/Generic_E24_1.list

IDIR="../../../mcad/vrml/thru/res/vert/MFR_1/0W50/0I20"

mkdir -p ${IDIR}/1R
mv MFR_?R*wrl ${IDIR}/1R

mkdir -p ${IDIR}/10R
mv MFR_??R*wrl ${IDIR}/10R

mkdir -p ${IDIR}/100R
mv MFR_???R*wrl ${IDIR}/100R

mkdir -p ${IDIR}/1K
mv MFR_?K*wrl ${IDIR}/1K

mkdir -p ${IDIR}/10K
mv MFR_??K*wrl ${IDIR}/10K

mkdir -p ${IDIR}/100K
mv MFR_???K*wrl ${IDIR}/100K

mkdir -p ${IDIR}/1M
mv MFR_?M*wrl ${IDIR}/1M

mkdir -p ${IDIR}/10M
mv MFR_??M*wrl ${IDIR}/10M


# Generic E48, 2% MFR, 0.5W 040 lead spacing, vertical
../../bin/makeMFR -p params/genericMFR_0W50_040v -o out_lists/Generic_E48_2.list

IDIR="../../../mcad/vrml/thru/res/vert/MFR_2/0W50/0I20"

mkdir -p ${IDIR}/1R
mv MFR_?R*wrl ${IDIR}/1R

mkdir -p ${IDIR}/10R
mv MFR_??R*wrl ${IDIR}/10R

mkdir -p ${IDIR}/100R
mv MFR_???R*wrl ${IDIR}/100R

mkdir -p ${IDIR}/1K
mv MFR_?K*wrl ${IDIR}/1K

mkdir -p ${IDIR}/10K
mv MFR_??K*wrl ${IDIR}/10K

mkdir -p ${IDIR}/100K
mv MFR_???K*wrl ${IDIR}/100K

mkdir -p ${IDIR}/1M
mv MFR_?M*wrl ${IDIR}/1M

mkdir -p ${IDIR}/10M
mv MFR_??M*wrl ${IDIR}/10M


# Generic E24, 5% CCR, 0.25W 040 lead spacing, vertical
../../bin/makeMFR -p params/genericCCR_0W25_040v -o out_lists/Generic_E24_5.list

IDIR="../../../mcad/vrml/thru/res/vert/CCR_5/0W25/0I20"

mkdir -p ${IDIR}/1R
mv CCR_?R*wrl ${IDIR}/1R

mkdir -p ${IDIR}/10R
mv CCR_??R*wrl ${IDIR}/10R

mkdir -p ${IDIR}/100R
mv CCR_???R*wrl ${IDIR}/100R

mkdir -p ${IDIR}/1K
mv CCR_?K*wrl ${IDIR}/1K

mkdir -p ${IDIR}/10K
mv CCR_??K*wrl ${IDIR}/10K

mkdir -p ${IDIR}/100K
mv CCR_???K*wrl ${IDIR}/100K

mkdir -p ${IDIR}/1M
mv CCR_?M*wrl ${IDIR}/1M

mkdir -p ${IDIR}/10M
mv CCR_??M*wrl ${IDIR}/10M

mkdir -p ${IDIR}/100M
mv CCR_???M*wrl ${IDIR}/100M

mkdir -p ${IDIR}/1G
mv CCR_?G*wrl ${IDIR}/1G
