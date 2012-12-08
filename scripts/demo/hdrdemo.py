# load the tools
import kc3d
from kc3d import *
import kc3dconn
from kc3dconn import *

out = ofstream()
hdr = Genhdr()
tx = Transform()
tx.setScale(0.3937)

# int SetPins(bool squarebot, bool squaretop, bool male, double pbev, double fbev,
#        double depth, double length, double pd0, double pdy, double pd1,
#        double pd2, double taper, double ts, int sides, double funneldepth);


# Male 6x2 header, rectangular beveled pins, 8mm high, 2.5mm pitch, beveled case
# TO CHECK: ensure that the pins are rectangular, beveled, and that the holes are beveled (no gap)
SetupVRML("testhdr_MS_6x2_8mm_bev.wrl", out)
hdr.setColors("../colors/black.mat", "../colors/gold.mat", "../colors/gold.mat", "../colors/black.mat")
hdr.setCase(6, 2, 2.54, 2.54, 2.72, .72, True, 0.4)
hdr.setPins(True, True, True, 0.1, -1, 2, 10, 0.64, 0.4, 0, 0, 0.3, 0.8, 0, 0)
hdr.build(tx, "HDR_MALE_SP_6x2_8MM", out, 0)
out.close()

# Female 6x2 header, rectangular beveled lower pins, circular top pins,
# 8mm high, 2.54mm pitch, beveled case, recessed top
# TO CHECK: sockets are recessed
kc3d.SetupVRML("testhdr_FSRD_6x2_8mm.wrl", out)
hdr.setColors("../colors/black.mat", "../colors/tin.mat", "../colors/gold.mat", "../colors/black.mat")
hdr.setCase(6, 2, 2.54, 2.54, 8, .72, True, 0.4)
hdr.setPins(True, False, False, 0.1, -1, 2, 10, 0.64, 0.4, 1.6, 0.96, 0.3, 0.8, 48, 0.25)
hdr.build(tx, "HDR_FEMALE_SRD_6x2_8MM", out, 0)
out.close()

# Female 6x2 header, rectangular beveled lower pins, circular top pins,
# 8mm high, 2.54mm pitch, beveled case, everted top
# TO CHECK: sockets are raised above the header, header is the correct height
kc3d.SetupVRML("testhdr_FSRU_6x2_8mm.wrl", out)
hdr.setColors("../colors/black.mat", "../colors/tin.mat", "../colors/gold.mat", "../colors/tin.mat")
hdr.setCase(6, 2, 2.54, 2.54, 8, .72, True, 0.4)
hdr.setPins(True, False, False, 0.1, -1, 2, 10, 0.64, 0.4, 1.6, 0.96, 0.3, 0.8, 48, -0.5)
hdr.build(tx, "HDR_FEMALE_SRU_6x2_8MM", out, 0)
out.close()

# Female 6x2 header, rectangular beveled lower pins, beveled square top pins,
# 8mm high, 2.54mm pitch, beveled case, recessed top
# TO CHECK: Funnels are beveled and the surrounding holes are correctly beveled
kc3d.SetupVRML("testhdr_FSSB_6x2_8mm.wrl", out)
hdr.setCase(6, 2, 2.54, 2.54, 8, .72, True, 0.4)
hdr.setPins(True, True, False, 0.1, 0.2, 2, 10, 0.64, 0.4, 1.6, 0.96, 0.3, 0.8, 16, 0.25)
hdr.build(tx, "HDR_FEMALE_SSB_6x2_8MM", out, 0)
out.close()

