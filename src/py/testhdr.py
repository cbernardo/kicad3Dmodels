# file: testhdr.py
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

# load the tools
import kc3d
import kc3dconn

# instantiate necessary objects
out = kc3d.ofstream()
hdr = kc3dconn.Genhdr()
tx = kc3d.Transform()
# set the scale to 1/2.54 so that the VRML units correspond to
# the 0.1 inch used by KiCAD.
tx.setScale(0.3937)

# create a male 6x2 header with square pins 8mm high, 2.54mm pitch, and beveled case
kc3d.SetupVRML("testhdr_MS_6x2_8mm.wrl", out)
hdr.setColors("../../mcad/colors/black.mat", "../../mcad/colors/gold.mat", "../../mcad/colors/gold.mat")
hdr.setCase(6, 2, 2.54, 2.54, 2.72, .72, 0.4)
hdr.setPins(1, 1, 2, 10, 0.64, 0.64, 0.64, 0.3, 0.8, 4, 0)
hdr.build(tx, "HDR_MALE_SP_6x2_8MM", out, 0);
out.close()

# create a female 6x2 header with square pins, 8mm high beveled case, 2.54mm pitch
kc3d.SetupVRML("testhdr_FS_6x2_8mm.wrl", out)
hdr.setColors("../../mcad/colors/black.mat", "../../mcad/colors/tin.mat", "../../mcad/colors/gold.mat")
hdr.setCase(6, 2, 2.54, 2.54, 8, .72, 0.4)
hdr.setPins(1, 0, 2, 10, 0.64, 1.6, 0.72, 0.3, 0.8, 24, 0.5)
hdr.build(tx, "HDR_FEMALE_SP_6x2_8MM", out, 0)
out.close()

# create a female 6x2 header with round pins, 8mm high beveled case, 2.54mm pitch
kc3d.SetupVRML("testhdr_FR_6x2_8mm.wrl", out)
hdr.setColors("../../mcad/colors/black.mat", "../../mcad/colors/tin.mat", "../../mcad/colors/gold.mat")
hdr.setCase(6, 2, 2.54, 2.54, 8, .72, 0.4)
hdr.setPins(0, 0, 2, 10, 0.64, 1.6, 0.96, 0.3, 0.8, 24, 0.1)
hdr.build(tx, "HDR_FEMALE_RP_6x2_8MM", out, 0);
out.close()


