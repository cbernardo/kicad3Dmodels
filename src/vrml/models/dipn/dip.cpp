/*
 *  file: dip.cpp
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

#include <cstdio>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <vdefs.h>
#include <vcom.h>
#include <transform.h>
#include <dip.h>
#include <dip_case.h>
#include <dip_pin.h>

using namespace std;
using namespace KC3D;
using namespace KC3DDIP;


DIPPARAMS::DIPPARAMS()
{
    // defaults are for DIP-24 with 0.3" row spacing and 0.1" pin-pin spacing
    A1  = 0.015;        // [BaseHeight] distance from board to bottom of case
    A2  = 0.115;        // [CaseDepth]
    L   = 0.15;         // [ThruLength] pin length (narrow part)
    e   = 0.1;          // [Pin Pitch]
    E   = 0.3;          // [RowSpacing] pin row spacing (for example, 0.3 inches on typical DIP)
    E1  = 0.25;         // [CaseWidth]
    B1  = 0.06;         // [PinBreadth] widest part of the pin
    b   = 0.022;        // [ThruWidth] width of narrow part of pin
    c   = 0.014;        // [Thickness] pin material thickness

    NW  = 0.06;         // [NotchWidth]
    ND  = 0.012;        // [NotchDepth]
    NL  = 0.07;         // [NotchLength] must be > NW/2
    MID = 0.022;        // [CaseMidHeight] depth of middle portion where pins attach
    DW  = 0.005;        // [CaseAlw]
    S = 0.005;          // [CaseTaper] deviation of unbeveled top and bottom edges
    casebev = 0.005;    // [CaseBevel]
    pinbev  = 0.002;    // Pin edge bevel

    scale = 10;
}


int DIP::SetPinColor( std::string filename )
{
    return pinmaterial.Load( filename );
}


int DIP::SetCaseColor( std::string filename )
{
    return casematerial.Load( filename );
}


DIP::DIP()
{
    pins = 0;
    haspin = NULL;
}


DIP::~DIP()
{
    if( (pins) && (haspin) )
        delete [] haspin;
}


int DIP::SetPins( int aNPins )
{
    if( (pins) && (haspin) )
        delete [] haspin;

    haspin = NULL;
    pins = 0;

    if( (aNPins < 4) || (aNPins % 2) )
    {
        ERRBLURB;
        cerr << "npins (" << aNPins << ") must be >= 4 and a multiple of 2\n";
        return -1;
    }

    haspin = new (nothrow) bool[aNPins];

    if( haspin == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate memory for pin inclusion map\n";
        return -1;
    }

    int i;

    for( i = 0; i < aNPins; ++i )
        haspin[i] = true;

    pins = aNPins;
    return 0;
}


int DIP::SetPin( int aPinNumber, bool isRendered )
{
    if( !pins )
    {
        ERRBLURB;
        cerr << "invoked without setting number of pins\n";
        return -1;
    }

    if( (aPinNumber < 1) || (aPinNumber > pins) )
    {
        ERRBLURB;
        cerr << "out of range (pin = " << aPinNumber;
        cerr << "); must be 1 .. pins (" << pins << ")\n";
        return -1;
    }

    haspin[aPinNumber - 1] = isRendered;
    return 0;
}


int DIP::Build( std::string aVRMLFilename )
{
    double xoff, yoff, zoff;
    int pin, hpin;
    ofstream fp;
    int acc = 0;

    if( pins == 0 )
    {
        ERRBLURB;
        cerr << "invoked without setting pins\n";
        return -1;
    }

    if( (pins < 4) || (pins % 2) )
    {
        ERRBLURB;
        cerr << "pins < 4 or not a multiple of 2: " << pins << "\n";
        return -1;
    }

    DIPCASE iccase;
    DIPPIN  icpin;

    const double tan5 = 0.0874886635259;

    if( params.DW < 0.0 )
    {
        params.DW = 2.0 * (params.c / 3.0 + 0.5 * params.B1);
    }

    if( params.MID < 0.0 )
    {
        params.MID = 1.5 * params.c;
    }

    if( params.S < 0.0 )
    {
        params.S = (params.A2 - params.MID) * 0.5 * tan5;
    }

    // length of case, nominal Pins/2 + DW
    double D = pins * 0.5 * params.e + 0.5 * params.DW;

    iccase.SetCaseLength( D );
    iccase.SetCaseWidth( params.E1 );
    iccase.SetBaseHeight( params.A1 );
    iccase.SetCaseDepth( params.A2 );
    iccase.SetCaseTaper( params.S );
    iccase.SetCaseMidHeight( params.MID );
    iccase.SetNotchLength( params.NL );
    iccase.SetNotchDepth( params.ND );

    // derived pin parameters
    double h = params.A1 + 0.5 * params.A2;
    double w = 0.5 * (params.E - params.E1);
    acc += icpin.Calc( h, params.L, w, params.c, params.B1, params.b, params.pinbev );

    if( acc )
    {
        ERRBLURB;
        cerr << "problems calculating pin appearance\n";
        return -1;
    }

    xoff = -(pins / 4.0 - 0.5) * params.e;
    yoff = -params.E1 * 0.5;
    zoff = 0.0;

    TRANSFORM T, TC;
    QUAT offset( 0, xoff, yoff, zoff );
    TRANSLATION tr( offset );
    ROTATION rot( 0, 0, 0, 1 );
    T.SetTranslation( tr * params.scale );
    T.SetRotation( rot );
    TC.SetScale( params.scale );

    acc += iccase.Calc();

    if( acc )
    {
        ERRBLURB;
        cerr << "problems calculating case appearance\n";
        return -1;
    }

    if( SetupVRML( aVRMLFilename, fp ) )
    {
        ERRBLURB;
        cerr << "could not open file '" << aVRMLFilename << "'\n";
        return -1;
    }

    // Case
    ostringstream partname;
    partname << "DIL" << pins << "_P" << setfill( '0' ) << setw( 5 ) <<
    (int) (params.E * 1000.0) << setfill( ' ' ) << setw( 0 );

    acc += SetupXForm( partname.str(), fp, 0 );

    acc += SetupShape( casematerial, false, fp, 2 );
    acc += iccase.WriteCoord( TC, fp, 4 );
    acc += iccase.WriteFacets( fp, 4 );
    acc += CloseShape( fp, 2 );

    if( acc )
    {
        ERRBLURB;
        cerr << "problems writing case data to file\n";
        return -1;
    }

    T.SetScale( params.scale );

    // Pin1
    if( haspin[0] )
        acc += icpin.Build( T, pinmaterial, false, fp, 2 );

    // Pin 2 .. (pins/2)
    hpin = pins / 2;

    for( pin = 2; pin <= hpin; ++pin )
    {
        offset.x = offset.x + params.e;

        if( !(haspin[pin - 1]) )
            continue;

        tr.Set( offset * params.scale );
        T.SetTranslation( tr );
        acc += icpin.Build( T, pinmaterial, true, fp, 2 );
    }

    // Pin (pins/2 +1)..
    rot.Set( M_PI, 0, 0, 1 );
    T.SetRotation( rot );
    offset.y = -offset.y;
    tr.Set( offset * params.scale );
    T.SetTranslation( tr );

    for( pin = hpin + 1; pin <= pins; ++pin )
    {
        if( haspin[pin - 1] )
            acc += icpin.Build( T, pinmaterial, true, fp, 2 );

        offset.x = offset.x - params.e;
        tr.Set( offset * params.scale );
        T.SetTranslation( tr );
    }

    if( CloseXForm( fp ) )
        return -1;

    if( acc )
        return -1;

    return 0;
}


int DIP::SetParams( const DIPPARAMS& aParams )
{
    if( aParams.pinbev > aParams.c / 3 )
    {
        ERRBLURB << "\n    + pin bevel ("<< aParams.pinbev;
        cerr << ") is wider than 1/3 the pin material thickness (" << aParams.c << ")\n";
        return -1;
    }

    if( aParams.b < aParams.c )
    {
        ERRBLURB << "\n    + pin thru width ("<< aParams.b;
        cerr << ") is less than the pin material thickness (" << aParams.c << ")\n";
        return -1;
    }

    if( aParams.b >= aParams.B1 )
    {
        ERRBLURB << "\n    + pin thru width ("<< aParams.b;
        cerr << ") is less than or equal to the pin breadth (" << aParams.B1 << ")\n";
        return -1;
    }

    // XXX - validate parameters

    params = aParams;
    return 0;
}
