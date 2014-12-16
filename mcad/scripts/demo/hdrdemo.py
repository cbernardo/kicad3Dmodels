# file: hdrdemo.py
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
#   Description: This script demonstrates the use of the generic
#   header tool. The models created are not realistic for actual
#   devices but they demonstrate some features and the flexibility
#   of the header tool.
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

# Male 6x2 header, rectangular beveled pins, 8mm high, 2.5mm pitch, beveled case
# TO CHECK: ensure that the pins are rectangular, beveled, and that the holes are beveled (no gap)
SetupVRML("testhdr_MS_6x2_8mm_bev.wrl", out)
hdr.setColors( os.path.join(matpath,"black.mat"), os.path.join(matpath,"gold.mat"), \
    os.path.join(matpath,"gold.mat"), os.path.join(matpath,"black.mat") )
hdr.setCase(6, 2, 2.54, 2.54, 2.72, .72, True, 0.4)
hdr.setPins(True, True, True, 0.1, -1, 2, 10, 0.64, 0.4, 0, 0, 0, 0, 0.3, 0.8, 0, 0)
hdr.build(tx, "HDR_MALE_SP_6x2_8MM", out, 0)
out.close()

# Female 6x2 header, rectangular beveled lower pins, circular top pins,
# 8mm high, 2.54mm pitch, beveled case, recessed top
# TO CHECK: sockets are recessed
kc3d.SetupVRML("testhdr_FSRD_6x2_8mm.wrl", out)
hdr.setColors( os.path.join(matpath,"black.mat"), os.path.join(matpath,"tin.mat"), \
    os.path.join(matpath,"gold.mat"), os.path.join(matpath,"black.mat") )
hdr.setCase(6, 2, 2.54, 2.54, 8, .72, True, 0.4)
hdr.setPins(True, False, False, 0.1, -1, 2, 10, 0.64, 0.4, 1.6, 0.96, 1.4, 1.1, 0.3, 0.8, 48, 0.25)
hdr.build(tx, "HDR_FEMALE_SRD_6x2_8MM", out, 0)
out.close()

# Female 6x2 header, rectangular beveled lower pins, circular top pins,
# 8mm high, 2.54mm pitch, beveled case, everted top
# TO CHECK: sockets are raised above the header, header is the correct height
kc3d.SetupVRML("testhdr_FSRU_6x2_8mm.wrl", out)
hdr.setColors( os.path.join(matpath,"black.mat"), os.path.join(matpath,"tin.mat"), \
    os.path.join(matpath,"gold.mat"), os.path.join(matpath,"tin.mat") )
hdr.setCase(6, 2, 2.54, 2.54, 8, .72, True, 0.4)
hdr.setPins(True, False, False, 0.1, -1, 2, 10, 0.64, 0.4, 1.6, 0.96, 1.4, 1.1, 0.3, 0.8, 48, -0.5)
hdr.build(tx, "HDR_FEMALE_SRU_6x2_8MM", out, 0)
out.close()

# Female 6x2 header, rectangular beveled lower pins, beveled square top pins,
# 8mm high, 2.54mm pitch, beveled case, recessed top
# TO CHECK: Funnels are beveled and the surrounding holes are correctly beveled
kc3d.SetupVRML("testhdr_FSSB_6x2_8mm.wrl", out)
hdr.setCase(6, 2, 2.54, 2.54, 8, .72, True, 0.4)
hdr.setPins(True, True, False, 0.1, 0.2, 2, 10, 0.64, 0.4, 1.6, 0.96, 1.4, 1.1, 0.3, 0.8, 16, 0.25)
hdr.build(tx, "HDR_FEMALE_SSB_6x2_8MM", out, 0)
out.close()

