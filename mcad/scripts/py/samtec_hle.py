# load the tools
import kc3d
from kc3d import *
import kc3dconn
from kc3dconn import *

out = ofstream()
hdr = Genhdr()
tx = Transform()
tx.setScale(0.3937)

# naming:
# HLE_[rows][pins per row]_02_[plating]_DV
# rows = 1,2
# pins = 2..50
# plating = X (2 options are visually indistinguishable - gold contact, matte tin on tail)
minpin = 8
maxpin = 8
PARTBASE = "samtec_HLE_1"
PARTEND  = "_02_X_DV"
for idx in range (minpin, maxpin):
    PARTNAME = PARTBASE + str(idx).zfill(2) + PARTEND
    FILENAME = PARTNAME + ".wrl"
    SetupVRML(FILENAME, out)
    out.close()
