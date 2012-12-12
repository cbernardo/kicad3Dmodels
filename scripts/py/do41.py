# load the tools
import kc3d
import kc3ddiode
from kc3d import *
from kc3ddiode import *

diode = GenDiode()
diode.setNVertices(16, 48, 5)
#DO41 has max material for dwire = 0.864, dbody = 2.72, lbody = 5.21
diode.setParams(0.864, 2.72, 5.21, 0.8, 0.6)
diode.setColors("../../../mcad/colors/tin.mat", "../../../mcad/colors/rcc_blk_g.mat", "../../../mcad/colors/rcc_wht_g.mat")

# horizontal orientation (pin 1 is always the cathode)
diode.build("do41_0I400H", 0.3937, True, False, 10.16, 4)
diode.build("do41_0I500H", 0.3937, True, False, 12.7, 4)
diode.build("do41_0I600H", 0.3937, True, False, 15.24, 4)
diode.build("do41_0I700H", 0.3937, True, False, 17.78, 4)
diode.build("do41_0I800H", 0.3937, True, False, 20.32, 4)
diode.build("do41_0I900H", 0.3937, True, False, 22.86, 4)
diode.build("do41_1I000H", 0.3937, True, False, 25.4, 4)

diode.setNVertices(16, 48, 10)

# vertical orientation, pin 1 is the cathode
diode.build("do41_0I100V", 0.3937, False, False, 2.54, 4)
diode.build("do41_0I200V", 0.3937, False, False, 5.08, 4)

# vertical orientation, pin 2 is the cathode
diode.build("do41_0I100VK2", 0.3937, False, True, 2.54, 4)
diode.build("do41_0I200VK2", 0.3937, False, True, 5.08, 4)
