# Script for generating default base filenames and color code sequences
# for input to makeMFR.

mkdir -p out_lists

# Generic MFR, 1%
../../bin/makeEnnn -p MFR -l 0 -u 7 -t F -e 96  > out_lists/Generic_E96_1.list
../../bin/makeEnnn -p MFR -l 0 -u 7 -t F -e 24  > out_lists/Generic_E24_1.list

# Generic MFR, 2%
../../bin/makeEnnn -p MFR -l 0 -u 7 -t G -e 96  > out_lists/Generic_E48_2.list

# Generic CCR, 5%
../../bin/makeEnnn -p CCR -l 0 -u 9 -t J -e 24  > out_lists/Generic_E24_5.list

