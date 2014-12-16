# file: samtec_ss_sd_ess_esd_hss.py
#
#   Copyright 2012-2014 Cirilo Bernardo
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>
#
#   Description: this script generates thru-hole headers from the
#   Samtec SS, SD, ESS, ESD, and HSS series.
#

# set up the environment
import os
import sys

if sys.version_info[0] > 2 or sys.version_info[1] < 2 :
    raise SystemExit('Sorry, this code needs Python >= 2.7 and < 3.0')

# set the material appearances path
matpath = os.path.realpath( os.path.dirname( os.path.realpath( sys.argv[0] ) ) )
( matpath, NULL ) = os.path.split( matpath )
( matpath, NULL ) = os.path.split( matpath )
matpath = os.path.join( matpath, "appearances" )

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

# SAMTEC series SS/ESS, SD/ESD part code:
# [series]-1[pins per row]-[plating]-[lead type]-[option 1]-[option 2]
# series:   SS/ESS = standard single / elevated single
#           SD/ESD = standard double / elevated double
#           HSS = high temp. SS - same pins as SS series but max 36 rather than 32
# pins per row: 01..32 (ss/ess), 01..36 (sd/esd/hss)
# plating: T = tin pin, gold contact (G/S/TT not generated so we can save space)
# lead type:
#   1A, 1B, 1C (ss/sd/hss) = pin depth of 9.14, 12.95, 6.60, square pin butting to circle: not modeled in Genhdr
#   2, 38 (ss/sd/hss) = pin 0.51 dia, depth 3.18 (4.57), total height 7.62 (8.89)
#   7, 5A (ss/sd/hss) = pin 0.64 dia (0.89), depth 10.41 (12.95), total height 15.24 (17.86)
#   03..08 (ess/esd): A (total height, B+C), B (height above board), C (pin depth below board)
#       0.64mm square pins
#       03: B (8.38), C (3.12)
#       04: 10.92, 3.12
#       05: 13.46, 4.39
#       06: 8.38, 5.66
#       07: 10.92, 6.93
#       08: 8.38, 9.47
# option 1 = L (locking socket) - not modeled by Genhdr
# option 2 = N (non flush) -

# mating series: TS, TD, HTS, BBS, BBD, BBL, BDL, BHS
# All mating series have 0.46mm dia. round pins so I assume a 0.66mm receptacle

# Assumptions about receptacle diameter:
# SS-T-2, SS-T-38: 0.66 (0.46 + 0.2)
# SS-T-7: 0.84 (0.64 + 0.2)
# SS-T-5A: 1.09 (0.89 + 0.2)
# ES[D]: 1.09 (to take 0.64 square pin or 0.89 round pin)

# sqrt(2)/2
K = 0.70710678

# Samtec SS (single row)
MINPIN = 1
MAXPIN = 33
PARTBASE = "samtec_SS_1"
PARTEND  = "_T_2_X_X"
DIM_B = 3.18
DIM_C = 7.62
PD = 0.51
RD = 0.66
FTC = 1.5
hdr.setColors( os.path.join(matpath,"black.mat"), os.path.join(matpath,"tin.mat"), \
              os.path.join(matpath,"gold.mat"), os.path.join(matpath,"tin.mat") )

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, DIM_C - DIM_B, DIM_C - DIM_B - 2.54, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, DIM_B, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.04)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_38_X_X"
DIM_B = 4.57
DIM_C = 8.89
PD = 0.51
RD = 0.66
FTC = 1.5

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, DIM_C - DIM_B, DIM_C - DIM_B - 2.54, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, DIM_B, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.04)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_7_X_X"
DIM_B = 10.41
DIM_C = 15.24
PD = 0.64
RD = 0.84
FTC = 1.3

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, DIM_C - DIM_B, DIM_C - DIM_B - 2.54, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, DIM_B, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.04)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_5A_X_X"
DIM_B = 12.95
DIM_C = 17.86
PD = 0.89
RD = 1.09
FTC = 1.2

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, DIM_C - DIM_B, DIM_C - DIM_B - 2.54, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, DIM_B, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.04)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec HSS (single row), 33 .. 36 pins only
MINPIN = 33
MAXPIN = 37
PARTBASE = "samtec_HSS_1"
PARTEND  = "_T_2_X_X"
DIM_B = 3.18
DIM_C = 7.62
PD = 0.51
RD = 0.66
FTC = 1.5
hdr.setColors( os.path.join(matpath,"black.mat"), os.path.join(matpath,"tin.mat"), \
              os.path.join(matpath,"gold.mat"), os.path.join(matpath,"tin.mat") )

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, DIM_C - DIM_B, DIM_C - DIM_B - 2.54, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, DIM_B, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.04)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_38_X_X"
DIM_B = 4.57
DIM_C = 8.89
PD = 0.51
RD = 0.66
FTC = 1.5

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, DIM_C - DIM_B, DIM_C - DIM_B - 2.54, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, DIM_B, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.04)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_7_X_X"
DIM_B = 10.41
DIM_C = 15.24
PD = 0.64
RD = 0.84
FTC = 1.3

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, DIM_C - DIM_B, DIM_C - DIM_B - 2.54, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, DIM_B, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.04)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_5A_X_X"
DIM_B = 12.95
DIM_C = 17.86
PD = 0.89
RD = 1.09
FTC = 1.2

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, DIM_C - DIM_B, DIM_C - DIM_B - 2.54, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, DIM_B, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.04)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec ESS (single row)
MINPIN = 1
MAXPIN = 33
PARTBASE = "samtec_ESS_1"
PARTEND  = "_T_03_X_X"
DIM_B = 8.38
DIM_C = 3.12
PD = 0.64
RD = 1.09
FTC = 1.2

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, DIM_B, 0, False, 0.3)
    hdr.setPins(True, False, False, -1, -1, DIM_C, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.76)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_04_X_X"
DIM_B = 10.92
DIM_C = 3.12
PD = 0.64
RD = 1.09
FTC = 1.2

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, DIM_B, 0, False, 0.3)
    hdr.setPins(True, False, False, -1, -1, DIM_C, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.76)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_05_X_X"
DIM_B = 13.46
DIM_C = 4.39
PD = 0.64
RD = 1.09
FTC = 1.2

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, DIM_B, 0, False, 0.3)
    hdr.setPins(True, False, False, -1, -1, DIM_C, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.76)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_06_X_X"
DIM_B = 8.38
DIM_C = 5.66
PD = 0.64
RD = 1.09
FTC = 1.2

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, DIM_B, 0, False, 0.3)
    hdr.setPins(True, False, False, -1, -1, DIM_C, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.76)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_07_X_X"
DIM_B = 10.92
DIM_C = 6.93
PD = 0.64
RD = 1.09
FTC = 1.2

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, DIM_B, 0, False, 0.3)
    hdr.setPins(True, False, False, -1, -1, DIM_C, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.76)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_08_X_X"
DIM_B = 8.38
DIM_C = 9.47
PD = 0.64
RD = 1.09
FTC = 1.2

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 1, 2.54, 2.54, DIM_B, 0, False, 0.3)
    hdr.setPins(True, False, False, -1, -1, DIM_C, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.76)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec SD (double row)
MINPIN = 1
MAXPIN = 37
PARTBASE = "samtec_SD_1"
PARTEND  = "_T_2_X_X"
DIM_B = 3.18
DIM_C = 7.62
PD = 0.51
RD = 0.64
FTC = 1.5
hdr.setColors( os.path.join(matpath,"black.mat"), os.path.join(matpath,"tin.mat"), \
              os.path.join(matpath,"gold.mat"), os.path.join(matpath,"tin.mat") )

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, DIM_C - DIM_B, DIM_C - DIM_B - 2.54, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, DIM_B, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.04)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_38_X_X"
DIM_B = 4.57
DIM_C = 8.89
PD = 0.51
RD = 0.64
FTC = 1.5

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, DIM_C - DIM_B, DIM_C - DIM_B - 2.54, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, DIM_B, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.04)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_7_X_X"
DIM_B = 10.41
DIM_C = 15.24
PD = 0.64
RD = 0.84
FTC = 1.3

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, DIM_C - DIM_B, DIM_C - DIM_B - 2.54, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, DIM_B, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.04)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_5A_X_X"
DIM_B = 12.95
DIM_C = 17.86
PD = 0.89
RD = 1.09
FTC = 1.2

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, DIM_C - DIM_B, DIM_C - DIM_B - 2.54, False, 0.3)
    hdr.setPins(False, False, False, -1, -1, DIM_B, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.04)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

# Samtec ESD (double row)
MINPIN = 1
MAXPIN = 37
PARTBASE = "samtec_ESD_1"
PARTEND  = "_T_03_X_X"
DIM_B = 8.38
DIM_C = 3.12
PD = 0.64
RD = 1.09
FTC = 1.2

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, DIM_B, 0, False, 0.3)
    hdr.setPins(True, False, False, -1, -1, DIM_C, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.76)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_04_X_X"
DIM_B = 10.92
DIM_C = 3.12
PD = 0.64
RD = 1.09
FTC = 1.2

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, DIM_B, 0, False, 0.3)
    hdr.setPins(True, False, False, -1, -1, DIM_C, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.76)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_05_X_X"
DIM_B = 13.46
DIM_C = 4.39
PD = 0.64
RD = 1.09
FTC = 1.2

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, DIM_B, 0, False, 0.3)
    hdr.setPins(True, False, False, -1, -1, DIM_C, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.76)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_06_X_X"
DIM_B = 8.38
DIM_C = 5.66
PD = 0.64
RD = 1.09
FTC = 1.2

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, DIM_B, 0, False, 0.3)
    hdr.setPins(True, False, False, -1, -1, DIM_C, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.76)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_07_X_X"
DIM_B = 10.92
DIM_C = 6.93
PD = 0.64
RD = 1.09
FTC = 1.2

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, DIM_B, 0, False, 0.3)
    hdr.setPins(True, False, False, -1, -1, DIM_C, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.76)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()

PARTEND  = "_T_08_X_X"
DIM_B = 8.38
DIM_C = 9.47
PD = 0.64
RD = 1.09
FTC = 1.2

for idx in range (MINPIN, MAXPIN):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    hdr.setCase(idx, 2, 2.54, 2.54, DIM_B, 0, False, 0.3)
    hdr.setPins(True, False, False, -1, -1, DIM_C, DIM_B + DIM_C, PD, PD, 1.83, RD, 1.32, FTC, 0.2, 0.6, 24, -0.76)
    hdr.build(tx, PARTNAME, out, 0)
    out.close()
