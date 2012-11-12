# Create a bent elliptical pin with tapered ends

import kc3d

out = kc3d.ofstream()
kc3d.SetupVRML("testpart.wrl", out)
pin = kc3d.Pin()

pp = kc3d.PParams()
pp.w = 1
pp.d = 2
pp.tap = 0.5
pp.stw = 1
pp.std = 0.8
pp.dbltap = 1
pp.h = 10
pp.r = 2
pp.nb = 5
pp.l = 10
pp.bend = 0.7

color = kc3d.VRMLMat()
color.Load("../../mcad/colors/gold.mat")

tx = kc3d.Transform()
pin.calc(pp, tx);

kc3d.SetupXForm("some_pin", out, 0)
pin.build(1, 1, tx, color, 0, out, 2)
kc3d.CloseXForm(out, 0)
out.close()

