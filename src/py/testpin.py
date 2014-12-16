# file: testpin.py
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
#   Description: create a bent elliptical pin with tapered ends to
#   demonstrate the use of the 'Pin' object.
#

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

