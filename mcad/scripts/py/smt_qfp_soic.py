# file: smt_qfp_soic.py
#
#   Copyright 2014 Cirilo Bernardo
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

#
#   Description: this script generates the following SMT packages:
#       QFP
#       SOIC
#       TSSOP
#   (not yet implemented: DFN/QFN)
#
#   Note: pin names in the VRML file does not correspond to pin numbering
#   of a device; the numbering is solely to provide a unique VRML ID.
#

#
#   Parameters:
#       [SECTION_NAME] = base name of output VRML file
#       pkgtype = [qfn | soic | tssop]
#       scale = scaling factor to convert length units to KiCad's VRML scale;
#               for example, if package units are in mm, set scale = 0.3937
#       pin_color = material file for pin color
#       body_color = material file for body color
#       body_size_x = width of the case body
#       body_size_y = length of the case body
#       body_size_z = depth of the case body
#       body_standoff = nominal body offset from PCB plane
#       pads_count_x = number of pads along each case width
#       pads_count_y = number of pads along each case length
#       pitch_x = pitch of pads along case width
#       pitch_y = pitch of pads along case length
#       lead_length = length of lead as measured from edge of case
#       lead_width = width of the lead
#       lead_thickness = material thickness of the lead
#       exp_pad_x = GND/heat tab width (0 = none)
#       exp_pad_y = GND/heat tab length (0 = none)
#
#   Note: the parameter file must conform to the Python ConfigParser
#   format. Default parameter values can be specified in a [DEFAULT]
#   section.
#

#
# Example file:
#
# [DEFAULT]
# pkgtype = none
# scale = 1.0
# pin_color = tin.mat
# body_color = charcoal.mat
#
# [SOIC-14_3.9x8.7mm_Pitch1.27mm]
# pkgtype = soic
# body_size_x = 1.53543
# body_size_y = 3.4055
# body_size_z = 0.51181
# body_standoff = 0.059055
# pads_count_x = 0
# pads_count_y = 7
# pitch_y = 0.499999
# pitch_x = %(pitch_y)s
# lead_length = 0.413385
# lead_width = 0.15748
# lead_thickness = 0.059055
# exp_pad_x = 0
# exp_pad_y = 0
#

# set up the environment
import os, ConfigParser
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


# sanitize a base name to create a suitable VRML name
def sanitize( aString ):
    if len( aString ) < 1:
        return

    for C in aString:
        if ( ( C < "a" or C > "z" ) and
            ( C < "A" or C > "Z" ) and
            ( C < "0" or C > "9" ) and
            ( C != "_" ) ) :
            aString = aString.replace( C, 'X' )

    if aString[0] >= "0" and aString[0] <= "9" :
        aString = "X_" + aString

    # ensure no VRML reserved words are used (case sensitive)
    if ( aString == "DEF" or aString == "EXTERNPROTO" or aString == "FALSE" or
        aString == "IS" or aString == "NULL" or aString == "PROTO" or
        aString == "ROUTE" or aString == "TO" or aString == "TRUE" or
        aString == "USE" or aString == "eventIn" or aString == "eventOut" or
        aString == "exposedField" or aString == "field" ):
        aString = "X_" + aString

    return aString

# define the pin geometry for QFN
def PinQFN( length, width ):
    tx = Transform()
    xs = SimplePoly()

    w2 = width / 2.0
    l2 = length - w2

    if length > width :
        # enumerate CCW for rendering pins
        xs.addVertex( w2, 0, 0 )
        da = math.pi / 7
        ang = 0
        i = 0

        while i < 8 :
            xs.addVertex( math.cos(ang) * w2, l2 + math.sin(ang) * w2, 0 )
            i += 1
            ang += da

        xs.addVertex( -w2, 0, 0 )

    else :
        xs.addVertex( w2, 0, 0 )
        xs.addVertex( w2, length, 0 )
        xs.addVertex( -w2, length, 0 )
        xs.addVertex( -w2, 0, 0 )

    return xs

# define the pin geometry for QFP
def PinQFP( length, width, thickness, height ):
    tx = Transform()
    pin = Wire()
    xs  = Rectangle()
#   Bevel pins are not such a big improvement in this case but double the file size
#    xs.setBevel( thickness * 0.1, 1 )   # bevel pins
    xs.calc( width, thickness, tx )
#   3 segments looks good enough and helps keep the file size smaller
    pin.setParams( 3, thickness  )      # nSeg per 90 deg bend, bend radius
    # default pin axis is on YZ plane
    thk2 = thickness / 2.0
    pin.addPoint( 0, -length, thk2 )
    pin.addPoint( 0, -length * 0.8, thk2 )
    pin.addPoint( 0, -length * 0.4, 1.5 * thk2 )    # typical bend for solder heel
    pin.addPoint( 0, -length * 0.4 + (height - thk2) * math.sin( math.pi * 5/180 ), height )
    pin.addPoint( 0, 0, height )
    return (pin, xs);


def HeatPad( out, tab_x, tab_y, standoff, pcolor ):
    if tab_x < 0.01 or tab_y < 0.01 :
        return
    t0 = Transform()
    t1 = Transform()
    p0 = Quat()
    t0.setTranslation( 0, 0, standoff )
    tab = SimplePoly()
    tx2 = tab_x / 2
    ty2 = tab_y / 2
    # CAVEAT: assuming we are creating output for KiCad and 1 VRML unit = 0.1inch,
    # a 0.35mm chamfer = 0.137795 units; however if this changes in the future
    # then we will have an incorrect chamfer here
    chamfer = 0.137795
    tab.addVertex( -tx2, ty2 - chamfer, 0 )
    tab.addVertex( -tx2, -ty2, 0 )
    tab.addVertex( tx2, -ty2, 0 )
    tab.addVertex( tx2, ty2, 0 )
    tab.addVertex( -tx2 + chamfer, ty2, 0 )
    tab.extrude( True, True, True, p0, t0, t1, pcolor, False, out, 1 )


def BodyQFP( out, body_size_x, body_size_y, body_size_z, body_standoff, lead_thickness, dotp, bcolor ):
    # case
    chamfer = body_size_z / 20
    xshrink = 1 - math.sin( math.pi / 30 ) * body_size_z / body_size_x
    yshrink = 1 - math.sin( math.pi / 30 ) * body_size_z / body_size_y
    xgrow = 1 / xshrink
    ygrow = 1 / yshrink
    xshrink2 = 1 - chamfer / body_size_x
    yshrink2 = 1 - chamfer / body_size_y
    xgrow2 = 1 / xshrink2
    ygrow2 = 1 / yshrink2
    body = Rectangle()
    body.setBevel( body_size_z/20, 1 ) # guess at generic bevel
    t0 = Transform()
    t0.setScale( xshrink*xshrink2, yshrink*yshrink2, 1 )
    t0.setRotation( 0, 0, 0, 1 )
    t0.setTranslation( 0, 0, body_standoff )
    body.calc( body_size_x, body_size_y, t0 )
    t0.setTranslation( 0, 0, 0 )
    t0.setScale( 1, 1, 1 )

    # bottom beveled extrusion
    z1 = body_standoff + chamfer
    t1 = Transform()
    t1.setTranslation( 0, 0, chamfer )
    t1.setScale( xgrow2, ygrow2, 1 )
    p0 = Quat()
    p0.w = 0
    p0.x = 0
    p0.y = 0
    p0.z = body_standoff
    body.extrude( True, False, True, p0, t1, t0, bcolor, False, out, 1 )

    # bottom draft extrusion
    body.xform( t1 )
    z2 = body_size_z / 2 + body_standoff - lead_thickness
    t1.setScale( xgrow, ygrow, 1 )
    t1.setTranslation( 0, 0, z2 - z1 )
    p0.z = z1
    body.extrude( False, False, True, p0, t1, t0, bcolor, True, out, 1 )

    # middle section extrusion
    z3 = z2 + 2 * lead_thickness
    body.xform( t1 )
    t1.setScale( 1, 1, 1 )
    t1.setTranslation( 0, 0, z3 - z2 )
    p0.z = z2
    body.extrude( False, False, True, p0, t1, t0, bcolor, True, out, 1 )

    # top draft extrusion
    z4 = body_standoff + body_size_z - chamfer
    body.xform( t1 )
    t1.setScale( xshrink, yshrink, 1 )
    t1.setTranslation( 0, 0, z4 - z3 )
    p0.z = z3
    body.extrude( False, False, True, p0, t1, t0, bcolor, True, out, 1 )

    # top chamfer extrusion
    body.xform( t1 )
    t1.setScale( xshrink2, yshrink2, 1 )
    t1.setTranslation( 0, 0, chamfer )
    p0.z = z4
    body.extrude( False, False, True, p0, t1, t0, bcolor, True, out, 1 )

    # Top of case
    t0 = Transform()
    t0.setScale( xshrink*xshrink2, yshrink*yshrink2, 1 )
    t0.setRotation( 0, 0, 0, 1 )
    t0.setTranslation( 0, 0, 0 )
    body.calc( body_size_x, body_size_y, t0 )
    tess = Tesselator()
    addPolygon( body, tess, False )
    t0.setScale( 1 );
    t0.setTranslation( 1.5*dotp - body_size_x/2, body_size_y/2 - 1.5*dotp, 0 )
    dot = Circle()
    dot.setNVertices( 12 )
    dot.calc( dotp, dotp, t0)
    addPolygon( dot, tess, True )
    t0.setTranslation( 0, 0, body_standoff + body_size_z )
    t0.setRotation( 0, 0, 0, 1 )
    tess.writeVRML( True, t0, bcolor, True, out, 1 )

    # place the dimple
    t0.setTranslation( 1.5*dotp - body_size_x/2, body_size_y/2 - 1.5*dotp, body_standoff + body_size_z )
    dimple = Dimple()
    dimple.setParams( dotp/2, dotp/8, 12, 5 )
    dimple.writeVRML( False, True, t0, bcolor, True, out, 1 )


def BodyQFN( out, body_size_x, body_size_y, body_size_z, body_standoff, lead_thickness, pin,
             pitch_x, pitch_y, pins_x, pins_y, bcolor ):
    tx = Transform()
    b1 = SimplePoly()
    bx2 = body_size_x / 2
    by2 = body_size_y / 2

    # start with top left and work counterclockwise
    b1.addVertex( -bx2, by2, body_standoff )

    # add left hand pins
    pt = 0;
    dy = (pins_y - 1) / 2.0 * pitch_y;
    tx.setRotation( -(math.pi / 2), 0, 0, 1 )
    while pt < pins_y :
        tx.setTranslation( -bx2, dy, body_standoff )
        dy -= pitch_y
        pt += 1
        r1 = SimplePoly( pin )
        r1.xform( tx )
        r1.reverse()
        pvsize = r1.getNVertices( )
        i = 0
        while i < pvsize :
            b1.addVertex( r1.getX(i), r1.getY(i), body_standoff )
            i += 1

    b1.addVertex( -bx2, -by2, body_standoff )

    # add front pins
    dx = -( (pins_x - 1) / 2.0 * pitch_x );
    tx.setRotation( 0, 0, 0, 1 )
    while pt < (pins_y + pins_x) :
        tx.setTranslation( dx, -by2, body_standoff )
        dx += pitch_x
        pt += 1
        r1 = SimplePoly( pin )
        r1.xform( tx )
        r1.reverse()
        pvsize = r1.getNVertices( )
        i = 0
        while i < pvsize :
            b1.addVertex( r1.getX(i), r1.getY(i), body_standoff )
            i += 1

    b1.addVertex( bx2, -by2, body_standoff )

    # add right hand pins
    dy = -( (pins_y - 1) / 2.0 * pitch_y );
    tx.setRotation( math.pi / 2, 0, 0, 1 )
    while pt < ( pins_y + pins_y + pins_x ):
        tx.setTranslation( bx2, dy, body_standoff )
        dy += pitch_y
        pt += 1
        r1 = SimplePoly( pin )
        r1.xform( tx )
        r1.reverse()
        pvsize = r1.getNVertices( )
        i = 0
        while i < pvsize :
            b1.addVertex( r1.getX(i), r1.getY(i), body_standoff )
            i += 1

    b1.addVertex( bx2, by2, body_standoff )

    # add back pins
    dx = (pins_x - 1) / 2.0 * pitch_x;
    tx.setRotation( math.pi, 0, 0, 1 )
    while pt < (pins_y + pins_y + pins_x + pins_x) :
        tx.setTranslation( dx, by2, body_standoff )
        dx -= pitch_x
        pt += 1
        r1 = SimplePoly( pin )
        r1.xform( tx )
        r1.reverse()
        pvsize = r1.getNVertices( )
        i = 0
        while i < pvsize :
            b1.addVertex( r1.getX(i), r1.getY(i), body_standoff )
            i += 1

    color = VRMLMat()
    color.Load( os.path.join( matpath, bcolor ) )

    tess = Tesselator()
    addPolygon( b1, tess, False )
    tx.setTranslation( 0, 0, body_standoff )
    tx.setRotation( 0, 0, 0, 1 )
    tess.writeVRML( False, tx, color, False, out, 1 )

    tx.setTranslation( 0, 0, 0 )
    b2 = SimplePoly( b1 )
    tx.setTranslation( 0, 0, lead_thickness )
    b2.xform( tx )
    tx.setTranslation( 0, 0, 0 );
    b1.stitch( True, b2, tx, color, True, out, 1 )

    # finish the case extrusion
    c1 = Rectangle()
    tx.setTranslation( 0, 0, lead_thickness )
    c1.calc( body_size_x, body_size_y, tx )
    tx.setTranslation( 0, 0, body_standoff + body_size_z - lead_thickness )
    t1 = Transform()
    p0 = Quat()
    p0.z = lead_thickness
    c1.extrude( True, True, True, p0, tx, t1, color, True, out, 1 )


def MakeQFN( out, body_size_x, body_size_y, body_size_z, body_standoff,
    pads_count_x, pads_count_y, pitch_x, pitch_y,
    lead_length, lead_width, lead_thickness,
    exp_pad_x, exp_pad_y, npcolor, nbcolor ):

    color = VRMLMat()   # pin color
    color.Load( os.path.join( matpath, npcolor ) )

    pin = PinQFN( lead_length, lead_width )

    BodyQFN( out, body_size_x, body_size_y, body_size_z, body_standoff,
            lead_thickness, pin, pitch_x, pitch_y, pads_count_x, pads_count_y,
            nbcolor )

    HeatPad( out, exp_pad_x, exp_pad_y, body_standoff, color )

    t0 = Transform()
    t1 = Transform()
    t1.setTranslation( 0, 0, lead_thickness )
    p0 = Quat()

    # Left hand side
    pcount = 1;
    limit = pitch_y * (pads_count_y - 1) / 2
    t0.setRotation( -math.pi / 2, 0, 0, 1 )
    hasColor = False

    while pcount <= pads_count_y :
        t0.setTranslation( -body_size_x / 2, limit, 0 );
        SetupXForm( "VPIN_%d" % pcount, out, 1 )
        pin.extrude( True, True, True, p0, t1, t0, color, False, out, 1 )
        hasColor = True
        CloseXForm( out, 1 )
        limit -= pitch_y
        pcount += 1

    # Front side
    limit = -pitch_x * (pads_count_x - 1) / 2
    t0.setRotation( 0, 0, 0, 1 )

    while pcount <= pads_count_y + pads_count_x:
        t0.setTranslation( limit, -body_size_y / 2, 0 );
        SetupXForm( "PIN_%d" % pcount, out, 1 )
        pin.extrude( True, True, True, p0, t1, t0, color, hasColor, out, 1 )
        CloseXForm( out, 1 )
        limit += pitch_x
        pcount += 1

    # Right hand side
    limit = -pitch_y * (pads_count_y - 1) / 2
    t0.setRotation( math.pi / 2, 0, 0, 1 )

    while pcount <= 2*pads_count_y + pads_count_x:
        t0.setTranslation( body_size_x / 2, limit, 0 );

        SetupXForm( "PIN_%d" % pcount, out, 1 )
        pin.extrude( True, True, True, p0, t1, t0, color, hasColor, out, 1 )
        CloseXForm( out, 1 )
        limit += pitch_y
        pcount += 1

    # Rear side
    limit = pitch_x * (pads_count_x - 1) / 2
    t0.setRotation( math.pi, 0, 0, 1 )

    while pcount <= 2*pads_count_y + 2*pads_count_x:
        t0.setTranslation( limit, body_size_y / 2, 0 );

        SetupXForm( "PIN_%d" % pcount, out, 1 )
        pin.extrude( True, True, True, p0, t1, t0, color, hasColor, out, 1 )
        CloseXForm( out, 1 )
        limit -= pitch_x
        pcount += 1


def MakeQFP( out, body_size_x, body_size_y, body_size_z, body_standoff,
    pads_count_x, pads_count_y, pitch_x, pitch_y,
    lead_length, lead_width, lead_thickness,
    exp_pad_x, exp_pad_y, npcolor, nbcolor ):

    color = VRMLMat()   # pin color
    color.Load( os.path.join( matpath, npcolor ) )
    bcolor = VRMLMat()  # body color
    bcolor.Load( os.path.join( matpath, nbcolor ) )

    dotp = min( body_size_x, body_size_y ) / 8

    # 0.31496 = 0.8mm / 2.54 (scale factor)
    if dotp > 0.31496 :
        dotp = 0.31496

    # 0.23622 = 0.6mm / 2,54
    #if dotp < 0.23622 :
    #    dotp = 0.23622

    # 0.1574 = 0.4mm / 2,54
    if dotp < 0.1574:
        dotp = 0.1574

    BodyQFP( out, body_size_x, body_size_y, body_size_z, body_standoff, lead_thickness, dotp, bcolor )

    HeatPad( out, exp_pad_x, exp_pad_y, body_standoff, color )

    (wire, section) = PinQFP( lead_length, lead_width, lead_thickness, body_size_z/2 + body_standoff )
    t0 = Transform()

    # Left hand side
    pcount = 1;
    limit = pitch_y * (pads_count_y - 1) / 2
    t0.setRotation( -math.pi / 2, 0, 0, 1 )
    hasColor = False

    while pcount <= pads_count_y :
        t0.setTranslation( -body_size_x / 2, limit, 0 );

        SetupXForm( "VPIN_%d" % pcount, out, 1 )
        wire.build( section, True, True, True, t0, color, hasColor, out, 1 )
        hasColor = True
        CloseXForm( out, 1 )
        limit -= pitch_y
        pcount += 1

    # Front side
    limit = -pitch_x * (pads_count_x - 1) / 2
    t0.setRotation( 0, 0, 0, 1 )

    while pcount <= pads_count_y + pads_count_x:
        t0.setTranslation( limit, -body_size_y / 2, 0 );

        SetupXForm( "PIN_%d" % pcount, out, 1 )
        wire.build( section, True, True, True, t0, color, hasColor, out, 1 )
        CloseXForm( out, 1 )
        limit += pitch_x
        pcount += 1

    # Right hand side
    limit = -pitch_y * (pads_count_y - 1) / 2
    t0.setRotation( math.pi / 2, 0, 0, 1 )

    while pcount <= 2*pads_count_y + pads_count_x:
        t0.setTranslation( body_size_x / 2, limit, 0 );

        SetupXForm( "PIN_%d" % pcount, out, 1 )
        wire.build( section, True, True, True, t0, color, hasColor, out, 1 )
        CloseXForm( out, 1 )
        limit += pitch_y
        pcount += 1

    # Rear side
    limit = pitch_x * (pads_count_x - 1) / 2
    t0.setRotation( math.pi, 0, 0, 1 )

    while pcount <= 2*pads_count_y + 2*pads_count_x:
        t0.setTranslation( limit, body_size_y / 2, 0 );

        SetupXForm( "PIN_%d" % pcount, out, 1 )
        wire.build( section, True, True, True, t0, color, hasColor, out, 1 )
        CloseXForm( out, 1 )
        limit -= pitch_x
        pcount += 1


def process( params ):
    cp = ConfigParser.ConfigParser()
    cp.read( params )
    out = ofstream()

    for X in cp.sections():
        print "Processing section: '%s'" % X
        fname = X + ".wrl"
        pkgtype = cp.get( X, "pkgtype" )
        scale = float( cp.get( X, "scale" ) )
        npcolor = cp.get( X, "pin_color" )
        nbcolor = cp.get( X, "body_color" )
        bs_x = float( cp.get( X, "body_size_x" ) )
        bs_y = float( cp.get( X, "body_size_y" ) )
        bs_z = float( cp.get( X, "body_size_z" ) )
        bstand = float( cp.get( X, "body_standoff" ) )
        pc_x = int( cp.get( X, "pads_count_x" ) )
        pc_y = int( cp.get( X, "pads_count_y" ) )
        pitch_y = float( cp.get( X, "pitch_y" ) )
        pitch_x = float( cp.get( X, "pitch_x" ) )
        lead_len = float( cp.get( X, "lead_length" ) )
        lead_wid = float( cp.get( X, "lead_width" ) )
        lead_thk = float( cp.get( X, "lead_thickness" ) )
        exp_pad_x = float( cp.get( X, "exp_pad_x" ) )
        exp_pad_y = float( cp.get( X, "exp_pad_y" ) )

        if scale != 1.0 :
            bs_x *= scale
            bs_y *= scale
            bs_z *= scale
            bstand *= scale
            pc_x *= scale
            pc_y *= scale
            pitch_y *= scale
            pitch_x *= scale
            lead_len *= scale
            lead_wid *= scale
            lead_thk *= scale
            exp_pad_x *= scale
            exp_pad_y *= scale

        # decide which model to invoke
        if ( pkgtype.lower() == "qfp" or pkgtype.lower() == "soic" or
            pkgtype.lower() == "tssop" ):
            SetupVRML( fname, out )
            SetupXForm( sanitize( X ), out, 0 )

            MakeQFP( out, bs_x, bs_y, bs_z, bstand,
                pc_x, pc_y, pitch_x, pitch_y,
                lead_len, lead_wid, lead_thk,
                exp_pad_x, exp_pad_y,
                npcolor, nbcolor )

            CloseXForm( out, 0 )
            out.close()
        elif pkgtype.lower() == "qfn" :
            SetupVRML( fname, out )
            SetupXForm( sanitize( X ), out, 0 )

            MakeQFN( out, bs_x, bs_y, bs_z, bstand,
                pc_x, pc_y, pitch_x, pitch_y,
                lead_len, lead_wid, lead_thk,
                exp_pad_x, exp_pad_y,
                npcolor, nbcolor )

            CloseXForm( out, 0 )
            out.close()
        else:
            sys.stderr.write("Unknown package (%s) in Section '%s'\n" % ( pkgtype, X ) )


##
##
##  MAIN CODE ENTRY POINT
##
##

if len( sys.argv ) == 1:
    print "No parameter files specified"
    exit()

for PARAMS in sys.argv[1:] :
    print "Processing file: %s" % PARAMS
    process( PARAMS )

exit()
