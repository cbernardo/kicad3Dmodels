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
#   Description: This script demonstrates the use of the Wire object
#   to create a coil. The output in this instance has a good visual
#   appearance but is probably not an ideal model to use in complex
#   projects due to the large vertex count.
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
import math
import kc3d
from kc3d import *

color = VRMLMat()

w = Wire()
w.setParams(8,1.0)
t0 = Transform()

out = ofstream()
# spiral coil
color.Load( os.path.join( matpath, "copper.mat") )
c = Circle()
c.setNVertices(48)
c.calc(1.0, 1.0, t0)
# Lead in
w.addPoint(0, 0, -5)
w.addPoint(0, 0, 0)
# Spiral
dx = 1.0/40.0;
x = dx;
h = math.sqrt(75.0)
while x < 5:
    y = 4.0*math.sin(x*math.pi*2.0)
    z = 5.0 - 4.0*math.cos(x*math.pi*2.0)
    w.addPoint(x, y, z)
    x += dx

# Lead out
w.addPoint(x, 0, 0)
w.addPoint(x, 0, -5)
SetupVRML("wiredemo_C.wrl", out)
SetupXForm("WIRE_DEMO", out, 0)
w.build(c, True, True, True, t0, color, False, out, 2)
w.clear()
CloseXForm(out, 0)
out.close()
