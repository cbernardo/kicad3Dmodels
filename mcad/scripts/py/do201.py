# file: do201.py
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
#   Description: this script generates a number of DO201 package diodes
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
import kc3ddiode
from kc3d import *
from kc3ddiode import *

diode = GenDiode()
diode.setNVertices(16, 48, 5)
#DO201 has max material for dwire = 1.3, dbody = 5.31, lbody = 9.5
diode.setParams(1.3, 5.31, 9.5, 1.0, 1.5)
diode.setColors( os.path.join(matpath,"tin.mat"), os.path.join(matpath,"rcc_blk_g.mat"), \
                os.path.join(matpath,"rcc_wht_g.mat") )

# horizontal orientation (pin 1 is always the cathode)
diode.build("do201_0I600H", 0.3937, True, False, 15.24, 4)
diode.build("do201_0I700H", 0.3937, True, False, 17.78, 4)
diode.build("do201_0I800H", 0.3937, True, False, 20.32, 4)
diode.build("do201_0I900H", 0.3937, True, False, 22.86, 4)
diode.build("do201_1I000H", 0.3937, True, False, 25.4, 4)

diode.setNVertices(16, 48, 10)

# vertical orientation, pin 1 is the cathode, short lead on K
diode.build("do201_0I200V", 0.3937, False, False, 5.08, 4)
diode.build("do201_0I300V", 0.3937, False, False, 7.62, 4)

# vertical orientation, pin 1 is the cathode, short lead on A
diode.build("do201_0I200VA", 0.3937, False, True, 5.08, 4)
diode.build("do201_0I300VA", 0.3937, False, True, 7.62, 4)
