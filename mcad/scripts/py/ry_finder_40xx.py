# file: ry_finder_40xx.py
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
#   Description: this script generates a Finder Relay 40.61 and can be
#   extended to generate a number of other relays from the 40.xx series.
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

import math
import kc3d
from kc3d import *

# pin size:
#   40.xx generally have pins 0.4mm along the length of the relay
#         and 1.0mm along the width.
#   40.11/41 relays have pins 0.5x1.0
#
# pin spacing:
#   40.xx has 20mm space from coil to COM, 5mm between COM/NO/NC
#         rows are spaced 7.5mm, coil is 3mm from LHS.
#         3-pin contacts: COM aligns with front coil pin,
#         and NO/NC align with rear coil pin.
#   40.11/41 has 3.5mm space between COM/NO/NC
#         contact rows are spaced 7.5mm, coil is 3mm from LHS,
#         coil pins are spaced 18mm on 40.11, 7.5 on 40.41.
#         NO/NC pins align with front coil pin on 40.11; on
#         40.41 the front coil pin aligns with COM.
#
# dimensions:
#   40.xx: 29 x 12.4 x 25
#   40.11: 28.5 x 26.3 x 12.7
#   40.41: 28.5 x 12.5 x 26.3

# 40.61
out = ofstream()

clr_base = VRMLMat()
clr_case = VRMLMat()
clr_pin  = VRMLMat()

clr_base.Load( os.path.join(matpath,"rcc_wht_g.mat") )
clr_case.Load( os.path.join(matpath,"glass_blu.mat") )
clr_pin.Load( os.path.join(matpath,"tin.mat") )

z0 = Transform()
z1 = Transform()
z2 = Transform()
tx = Transform()

z1.setTranslation( 0, 0, 1 )
z2.setTranslation( 0, 0, 25 )
tx.setScale( 0.3937 )

r1 = Rectangle()
r2 = Rectangle()
r3 = Rectangle()

r1.setBevel( 0.5, 1 )
r2.setBevel( 0.5, 1 )
r3.setBevel( 0.5, 1 )

r1.calc( 29, 12.4, z0 )
r2.calc( 29, 12.4, z1 )
r3.calc( 29, 12.4, z2 )

SetupVRML( "ry_finder_4061.wrl", out )
SetupXForm( "RY_FINDER_4061", out, 0 )

# base
r1.paint( False, tx, clr_base, False, out, 1 )
r2.paint( True, tx, clr_base, True, out, 1 )
r1.stitch( True, r2, tx, clr_base, True, out, 1 )

# case
r3.paint( True, tx, clr_case, False, out, 1 )
r2.stitch( True, r3, tx, clr_case, True, out, 1 )
z2.setTranslation( 0, 0, 24.5 )
r2.calc( 28.2, 11.6, z1 )
r3.calc( 28.2, 11.6, z2 )
r3.paint( True, tx, clr_case, True, out, 1 )
r2.stitch( True, r3, tx, clr_case, True, out, 1 )

#pins
p0 = Pin()
pp0 = PParams()

pp0.w = 0.4
pp0.d = 1.0
pp0.h = 5.3
pp0.l = -1
pp0.bend = 0
pp0.r = -1
pp0.tap = 0.3
pp0.dbltap = False
pp0.stw = 0.8
pp0.std = 0.92
pp0.bev = 0

p0.setShape( True )
p0.calc( pp0, z0 )
# coil pins
tx.setTranslation( -11.5, -3.25, -5.3 )
p0.build( True, False, tx, clr_pin, False, out, 1 )
tx.setTranslation( -11.5, 3.25, -5.3 )
p0.build( True, False, tx, clr_pin, True, out, 1 )
# NC pins
tx.setTranslation( 3.5, -3.25, -5.3 )
p0.build( True, False, tx, clr_pin, True, out, 1 )
tx.setTranslation( 3.5, 3.25, -5.3 )
p0.build( True, False, tx, clr_pin, True, out, 1 )
# COM pins
tx.setTranslation( 8.5, -3.25, -5.3 )
p0.build( True, False, tx, clr_pin, True, out, 1 )
tx.setTranslation( 8.5, 3.25, -5.3 )
p0.build( True, False, tx, clr_pin, True, out, 1 )
# NO pins
tx.setTranslation( 13.5, -3.25, -5.3 )
p0.build( True, False, tx, clr_pin, True, out, 1 )
tx.setTranslation( 13.5, 3.25, -5.3 )
p0.build( True, False, tx, clr_pin, True, out, 1 )

# attempt at a coil
clr_coil  = VRMLMat()
clr_blk   = VRMLMat()

clr_coil.Load( os.path.join(matpath,"copper.mat") )
clr_blk.Load( os.path.join(matpath,"black.mat") )

tx.setTranslation( 0, 0, 0 )
z0.setTranslation( -7.5, 0, 1 )
z1.setTranslation( -7.5, 0, 20 )
z2.setTranslation( -7.5, 0, 20.5 )

c0 = Circle()
c1 = Circle()

c0.setNVertices( 48 )
c1.setNVertices( 48 )

c0.calc( 10, 10, z0 )
c1.calc( 10, 10, z1 )

c0.stitch( True, c1, tx, clr_coil, False, out, 1 )

c0.calc( 10.5, 10.5, z1 )
c1.calc( 10.5, 10.5, z2 )

c0.stitch( True, c1, tx, clr_blk, False, out, 1 )
c0.paint( False, tx, clr_blk, True, out, 1 )
c1.paint( True, tx, clr_blk, True, out, 1 )

# pretend contacts
z0.setTranslation( 3.5, 0, 1 )
z1.setTranslation( 3.5, 0, 20 )
z2.setTranslation( 3.5, 0, 20.5 )

r1.setBevel( 0.2, 1 )
r2.setBevel( 0.2, 1 )
r3.setBevel( 0.2, 1 )

r1.calc( 1, 10, z0 )
r2.calc( 1, 10, z1 )
r3.calc( 0.6, 9.6, z2 )

r3.paint( True, tx, clr_coil, False, out, 1 )
r1.stitch( True, r2, tx, clr_coil, True, out, 1 )
r2.stitch( True, r3, tx, clr_coil, True, out, 1 )

tx.setTranslation( 5, 0, 0 )
r3.paint( True, tx, clr_coil, True, out, 1 )
r1.stitch( True, r2, tx, clr_coil, True, out, 1 )
r2.stitch( True, r3, tx, clr_coil, True, out, 1 )

tx.setTranslation( 10, 0, 0 )
r3.paint( True, tx, clr_coil, True, out, 1 )
r1.stitch( True, r2, tx, clr_coil, True, out, 1 )
r2.stitch( True, r3, tx, clr_coil, True, out, 1 )


CloseXForm( out, 0 )
out.close()
