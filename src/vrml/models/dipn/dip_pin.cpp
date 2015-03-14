/*
 *  file: dip_pin.cpp
 *
 *  Copyright 2012-2014 Cirilo Bernardo (cjh.bernardo@gmail.com)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

#include <fstream>
#include <iostream>
#include <cmath>
#include <iomanip>

#include <vdefs.h>
#include <dip_pin.h>
#include <transform.h>
#include <vrmlmat.h>
#include <pin.h>

using namespace std;
using namespace KC3D;
using namespace KC3DDIP;

// the radius coefficient; R = RC*c
#define RC (1.0)

DIPPIN::DIPPIN()
{
    valid = false;
}


// Calculate the vertices
int DIPPIN::Calc( double aPinHeight, double aPinDepth, double aPinHLength,
                  double aPinThickness, double aPinBreadth, double aThruBreadth,
                  double aPinBevel )
{
    valid = false;
    double r = RC * aPinThickness;

    if( aPinDepth <= 0.0 )
    {
        ERRBLURB;
        cerr << "aPinDepth must be > 0\n";
        return -1;
    }

    if( aPinThickness <= 0.0 )
    {
        ERRBLURB;
        cerr << "aPinThickness must be > 0\n";
        return -1;
    }

    if( aPinBreadth <= 0.0 )
    {
        ERRBLURB;
        cerr << "aPinBreadth must be > 0\n";
        return -1;
    }

    if( (aThruBreadth <= 0.0) || (aThruBreadth >= aPinBreadth) )
    {
        ERRBLURB;
        cerr << "aThruBreadth must be > 0 and < aPinBreadth\n";
        return -1;
    }

    // check if the bevel interferes with features
    if( (aPinBevel > aPinThickness / 4.0) || (aPinBevel > aThruBreadth / 4.0) )
    {
        ERRBLURB;
        cerr << "aPinBevel must be < aPinThickness/4 and < aThruBreadth/4\n";
        return -1;
    }

    if( aPinHLength < r )
    {
        ERRBLURB;
        cerr << "pin extent (aPinHLength = " << setprecision( 8 ) << aPinHLength <<
            ") is less than mandated radius (r = " << RC << " * aPinThickness = "
            << r << ")\n";
        return -1;
    }

    const double tan40 = 0.839099631177;
    double taper = (aPinBreadth - aThruBreadth) / 2.0 * tan40;

    if( aPinHeight <= (r + taper) )
    {
        ERRBLURB;
        cerr << "pin's attachment height (aPinHeight) must be > (" << RC
            << "*t + (pb - pt)/2*tan(40))\n";
        return -1;
    }

    TRANSFORM t0;
    t0.SetTranslation( 0, -aPinHLength, -aPinDepth );
    t0.SetRotation( M_PI * 0.5, 0, 0, 1 );

    PPARAMS pp;
    pp.bev  = aPinBevel;
    pp.d    = aThruBreadth;
    pp.w    = aPinThickness;
    pp.dbltap = false;
    pp.h = aPinDepth;
    pp.r = -0.1;
    pp.l = -0.1;
    pp.tap = aPinBevel;

    if( aPinBevel > 0 )
    {
        pp.stw  = 1.0 - 2.0 * aPinBevel / aPinThickness;
        pp.std  = 1.0 - 2.0 * aPinBevel / aThruBreadth;
    }

    int acc = 0;
    acc += pin[0].Calc( pp, t0 );

    t0.SetTranslation( 0, -aPinHLength, 0 );
    pp.d = aPinBreadth;
    pp.h = aPinHeight - r;
    pp.tap = taper;
    pp.r = r;
    pp.bend = 0.5 * M_PI;
    pp.nb   = 3;
    pp.l    = aPinHLength - r;
    pp.stw  = 1.0;
    pp.std  = aThruBreadth / aPinBreadth;
    acc += pin[1].Calc( pp, t0 );

    if( acc )
    {
        ERRBLURB;
        cerr << "problems calculating pin vertices\n";
        return -1;
    }

    valid = true;

    return 0;
}


// Write the shape information to a file
int DIPPIN::Build( KC3D::TRANSFORM& aTransform, KC3D::VRMLMAT& aPinMaterial,
                   bool reuseMaterial, std::ofstream& aVRMLFile, int aTabDepth )
{
    if( !valid )
    {
        ERRBLURB;
        cerr << "invoked with no prior successful call to Calc()\n";
        return -1;
    }

    int acc = 0;
    acc += pin[0].Build( true, false, aTransform, aPinMaterial, reuseMaterial,
                         aVRMLFile, aTabDepth );
    acc += pin[1].Build( false, false, aTransform, aPinMaterial, true,
                         aVRMLFile, aTabDepth );

    if( acc )
    {
        ERRBLURB;
        cerr << "problems writing pin data to file\n";
        return -1;
    }

    return 0;
}
