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

# SAMTEC series SL/SLD part code:
# [series]-1[pins per row]-[plating]-[lead type]-[option 1]-[option 2]
# series:   SL/SLD = low profile single / low profile double
# pins per row: 01..32 (sl), 01..36 (sld)
# plating: T = tin pin, gold contact (G/S/TT not generated so we can save space)
# lead type:
#   10: low profile. hollow leg
#   11: low profile standard mount
#   12: micro-socket, hollow leg
#   19: micro-socket, hollow leg

# Samtec SL -10 (single row)
MINPIN = 2
MAXPIN = 33
PARTBASE = "samtec_SL_1"
PARTEND  = "_T_10"
P0 = 0.76
H0 = 5.84
H1 = 3.10
H2 = 0.76
hdr.setColors("../../../mcad/colors/black.mat", "../../../mcad/colors/tin.mat", "../../../mcad/colors/gold.mat", "../../../mcad/colors/tin.mat")
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, H1, H1 - H2 - 1.27, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, H0 - H1, H0, P0, P0, 1.83, P0 + 0.2, 1.32, 1.2, 0.2, 0.6, 24, -H2)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec SL -11 (single row)
PARTBASE = "samtec_SL_1"
PARTEND  = "_T_11"
P0 = 0.51
H0 = 6.10
H1 = 3.35
H2 = 0.51
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, H1, H1 - H2 - 1.27, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, H0 - H1, H0, P0, P0, 1.83, P0 + 0.2, 1.32, 1.5, 0.2, 0.6, 24, -H2)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec SL -12 (single row)
PARTBASE = "samtec_SL_1"
PARTEND  = "_T_12"
P0 = 0.76
H0 = 4.83
H1 = 2.41
H2 = 0.51
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, H1, H1 - H2 - 1.27, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, H0 - H1, H0, P0, P0, 1.83, P0 + 0.2, 1.32, 1.2, 0.2, 0.6, 24, -H2)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec SL -19 (single row)
PARTBASE = "samtec_SL_1"
PARTEND  = "_T_19"
P0 = 0.74
H0 = 4.19
H1 = 2.11
H2 = 0.25
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, H1, H1 - H2 - 1.27, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, H0 - H1, H0, P0, P0, 1.83, P0 + 0.2, 1.32, 1.2, 0.2, 0.6, 24, -H2)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

    
    
    

# Samtec SLD -10 (double row)
MINPIN = 2
MAXPIN = 37
PARTBASE = "samtec_SLD_1"
PARTEND  = "_T_10"
P0 = 0.76
H0 = 5.84
H1 = 3.10
H2 = 0.76
hdr.setColors("../../../mcad/colors/black.mat", "../../../mcad/colors/tin.mat", "../../../mcad/colors/gold.mat", "../../../mcad/colors/tin.mat")
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, H1, H1 - H2 - 1.27, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, H0 - H1, H0, P0, P0, 1.83, P0 + 0.2, 1.32, 1.2, 0.2, 0.6, 24, -H2)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec SLD -11 (double row)
PARTBASE = "samtec_SLD_1"
PARTEND  = "_T_11"
P0 = 0.51
H0 = 6.10
H1 = 3.35
H2 = 0.51
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, H1, H1 - H2 - 1.27, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, H0 - H1, H0, P0, P0, 1.83, P0 + 0.2, 1.32, 1.5, 0.2, 0.6, 24, -H2)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec SLD -12 (double row)
PARTBASE = "samtec_SLD_1"
PARTEND  = "_T_12"
P0 = 0.76
H0 = 4.83
H1 = 2.41
H2 = 0.51
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, H1, H1 - H2 - 1.27, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, H0 - H1, H0, P0, P0, 1.83, P0 + 0.2, 1.32, 1.2, 0.2, 0.6, 24, -H2)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec SLD -19 (double row)
PARTBASE = "samtec_SLD_1"
PARTEND  = "_T_19"
P0 = 0.74
H0 = 4.19
H1 = 2.11
H2 = 0.25
for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, H1, H1 - H2 - 1.27, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, H0 - H1, H0, P0, P0, 1.83, P0 + 0.2, 1.32, 1.2, 0.2, 0.6, 24, -H2)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()
