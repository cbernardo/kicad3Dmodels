# load the tools
import kc3d
import kc3ddiode
from kc3d import *
from kc3ddiode import *

diode = GenDiode()
diode.setNVertices(16, 48, 5)
#DO204 has max material for dwire = 1.32, dbody = 6.35, lbody = 9.52
diode.setParams(1.32, 6.35, 9.52, 1.5, 0.4)
diode.setColors("../../../mcad/colors/tin.mat", "../../../mcad/colors/rcc_blk_g.mat", "../../../mcad/colors/rcc_wht_g.mat")

# horizontal orientation (pin 1 is always the cathode)
diode.build("do204_0I600H", 0.3937, True, False, 15.24, 4)
diode.build("do204_0I700H", 0.3937, True, False, 17.78, 4)
diode.build("do204_0I800H", 0.3937, True, False, 20.32, 4)
diode.build("do204_0I900H", 0.3937, True, False, 22.86, 4)
diode.build("do204_1I000H", 0.3937, True, False, 25.4, 4)

diode.setNVertices(16, 48, 10)

# vertical orientation, pin 1 is the cathode
diode.build("do204_0I200V", 0.3937, False, False, 5.08, 4)
diode.build("do204_0I300V", 0.3937, False, False, 7.62, 4)

# vertical orientation, pin 2 is the cathode
diode.build("do204_0I200VK2", 0.3937, False, True, 5.08, 4)
diode.build("do204_0I300VK2", 0.3937, False, True, 7.62, 4)
