# load the tools
import kc3d
from kc3d import *
import kc3dconn
from kc3dconn import *

out = ofstream()
hdr = Genhdr()
tx = Transform()
tx.setScale(0.3937)

#    int SetPins(bool squarebot, bool squaretop, bool male, double pbev, double fbev,
#            double depth, double length, double pd0, double pdy, double pd1,
#            double pd2, double pd3, double ftc, double taper, double ts, int sides, double funneldepth);

# Samtec series TSW (2..40 pin SIL header)
# There are many options not implemented here such as '-2xx' which has a pin every other hole,
# -T which has 3 rows, and -Q which has 3 rows with the center row blank, and the right angle versions.
# Other omissions include lead styles -26 (round pin on bottom) and 41, 42 (HTSW versions only)
#
# [series]-1[pins per row]-[lead type]-[plating]-[row option]-[other option]
# series:   TSW
# pins per row: 01..50 (for straight thru)
# plating: G = gold plate on top, gold flash on remainder
# row option: S(ingle), D(ouble) [T and Q not implemented here]
# insulator: 2.54mm height, 0.4mm shoulder, 0.5mm wide, square profile
# lead style: first number is pin depth; second is total pin length
#   05: 3.30    8.51
#   06: 2.41    7.62
#   07: 2.54    10.92
#   08: 5.08    13.46
#   09: 10.16   18.54
#   10: 12.70   21.08
#   11: 15.24   23.62
#   12: 17.78   26.16
#   13: 22.86   31.24
#   14: 2.79    13.46
#   15: 2.79    18.54
#   16: 7.87    18.54
#   17: 2.79    21.08
#   18: 2.79    23.62
#   19: 2.79    26.16
#   20: 2.79    31.24
#   21: 2.79    36.32
#   22: 7.62    16.00
#   23: 2.92    11.30
#   24: 2.92    12.15
#   25: 5.33    16.00
#   27: 25.4    33.78
#   28: 20.32   28.70
#   29: 23.11   33.78
#   30: 18.03   28.70

MINPIN = 1
MAXPIN = 50
hdr.setColors("../../../mcad/colors/black.mat", "../../../mcad/colors/gold.mat", "../../../mcad/colors/gold.mat", "../../../mcad/colors/gold.mat")

# Samtec TSW-1xx-05-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_05_G_S_XX"
H0 = 3.30
H1 = 8.51
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-05-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_05_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   06: 2.41    7.62
# Samtec TSW-1xx-06-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_06_G_S_XX"
H0 = 2.41
H1 = 7.62
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-06-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_06_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   07: 2.54    10.92
# Samtec TSW-1xx-07-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_07_G_S_XX"
H0 = 2.54
H1 = 10.92
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-07-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_07_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   08: 5.08    13.46
# Samtec TSW-1xx-08-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_08_G_S_XX"
H0 = 5.08
H1 = 13.46
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-08-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_08_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   09: 10.16   18.54
# Samtec TSW-1xx-09-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_09_G_S_XX"
H0 = 10.16
H1 = 18.54
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-09-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_09_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   10: 12.70   21.08
# Samtec TSW-1xx-10-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_10_G_S_XX"
H0 = 12.70
H1 = 21.08
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-10-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_10_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   11: 15.24   23.62
# Samtec TSW-1xx-11-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_11_G_S_XX"
H0 = 15.24
H1 = 23.62
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-11-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_11_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   12: 17.78   26.16
# Samtec TSW-1xx-12-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_12_G_S_XX"
H0 = 17.78
H1 = 26.16
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-12-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_12_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   13: 22.86   31.24
# Samtec TSW-1xx-13-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_13_G_S_XX"
H0 = 22.86
H1 = 31.24
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-13-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_13_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   14: 2.79    13.46
# Samtec TSW-1xx-14-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_14_G_S_XX"
H0 = 2.79
H1 = 13.46
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-14-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_14_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   15: 2.79    18.54
# Samtec TSW-1xx-15-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_15_G_S_XX"
H0 = 2.79
H1 = 18.54
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-15-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_15_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   16: 7.87    18.54
# Samtec TSW-1xx-16-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_16_G_S_XX"
H0 = 7.87
H1 = 18.54
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-16-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_16_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   17: 2.79    21.08
# Samtec TSW-1xx-17-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_17_G_S_XX"
H0 = 2.79
H1 = 21.08
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-17-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_17_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   18: 2.79    23.62
# Samtec TSW-1xx-18-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_18_G_S_XX"
H0 = 2.79
H1 = 23.62
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-18-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_18_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   19: 2.79    26.16
# Samtec TSW-1xx-19-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_19_G_S_XX"
H0 = 2.79
H1 = 26.16
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-19-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_19_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   20: 2.79    31.24
# Samtec TSW-1xx-20-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_20_G_S_XX"
H0 = 2.79
H1 = 31.24
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-20-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_20_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   21: 2.79    36.32
# Samtec TSW-1xx-21-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_21_G_S_XX"
H0 = 2.79
H1 = 36.32
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-21-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_21_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   22: 7.62    16.00
# Samtec TSW-1xx-22-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_22_G_S_XX"
H0 = 7.62
H1 = 16.00
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-22-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_22_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   23: 2.92    11.30
# Samtec TSW-1xx-23-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_23_G_S_XX"
H0 = 2.92
H1 = 11.30
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-23-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_23_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   24: 2.92    12.15
# Samtec TSW-1xx-24-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_24_G_S_XX"
H0 = 2.92
H1 = 12.15
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-24-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_24_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   25: 5.33    16.00
# Samtec TSW-1xx-25-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_25_G_S_XX"
H0 = 5.33
H1 = 16.00
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-25-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_25_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   27: 25.4    33.78
# Samtec TSW-1xx-27-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_27_G_S_XX"
H0 = 25.4
H1 = 33.78
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-27-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_27_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   28: 20.32   28.70
# Samtec TSW-1xx-28-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_28_G_S_XX"
H0 = 20.32
H1 = 28.70
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-28-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_28_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   29: 23.11   33.78
# Samtec TSW-1xx-29-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_29_G_S_XX"
H0 = 23.11
H1 = 33.78
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-29-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_29_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

#   30: 18.03   28.70
# Samtec TSW-1xx-30-S-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_30_G_S_XX"
H0 = 18.03
H1 = 28.70
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec TSW-1xx-30-D-XX
PARTBASE = "samtec_TSW_1"
PARTEND  = "_30_G_D_XX"
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, 2.54, 0.4, True, 0.5)
    hdr.setPins(True, True, True, -1, -1, H0, H1, 0.64, 0.64, 0, 0, 0, 0, 0.3, 0.4, 4, 0)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()
