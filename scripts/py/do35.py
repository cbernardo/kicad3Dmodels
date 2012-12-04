# load the tools
import kc3d
import kc3ddiode
from kc3d import *
from kc3ddiode import *

diode = do35()
diode.setNVertices(16, 48, 5)
diode.setColors("../../../mcad/colors/tin.mat", "../../../mcad/colors/glass_clr.mat", "../../../mcad/colors/glass_blk.mat", "../../../mcad/colors/copper.mat")

# horizontal orientation (pin 1 is always the cathode)
diode.build("do35_0I300H", 0.3937, True, False, 7.62)
diode.build("do35_0I400H", 0.3937, True, False, 10.16)
diode.build("do35_0I500H", 0.3937, True, False, 12.7)
diode.build("do35_0I600H", 0.3937, True, False, 15.24)
diode.build("do35_0I700H", 0.3937, True, False, 17.78)
diode.build("do35_0I800H", 0.3937, True, False, 20.32)
diode.build("do35_0I900H", 0.3937, True, False, 22.86)
diode.build("do35_1I000H", 0.3937, True, False, 25.4)

diode.setNVertices(16, 48, 10)

# vertical orientation, pin 1 is the cathode
diode.build("do35_0I100V", 0.3937, False, False, 2.54)
diode.build("do35_0I200V", 0.3937, False, False, 5.08)

# vertical orientation, pin 2 is the cathode
diode.build("do35_0I100VK2", 0.3937, False, True, 2.54)
diode.build("do35_0I200VK2", 0.3937, False, True, 5.08)
