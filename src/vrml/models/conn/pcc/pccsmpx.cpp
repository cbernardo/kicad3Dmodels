/*
 *      file: pccsmpx.cpp
 *
 *      Copyright 2012-2014 Cirilo Bernardo (cjh.bernardo@gmail.com)
 *
 *      This program is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

/*
 *  This program generates the PCC-SMP-X miniature horizontal mounting
 *  glass-filled nylon thermocouple connector
 */


#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <wx/filename.h>

#include <macros.h>
#include <vdefs.h>
#include <keyvalparser.h>
#include <transform.h>
#include <vrmlmat.h>
#include <vcom.h>

#include <pin.h>
#include <rectangle.h>
#include <circle.h>
#include <funnel.h>
#include <hole.h>

#include <k3dconfig.h>

using namespace std;
using namespace KC3D;

extern char* optarg;
extern int optopt;

void printUsage( void )
{
    cout << "Usage: makePCC-SMP [-k appearances_path]\n";
    cout << "When invoked without a path specification, the environment\n";
    cout << "variable KC3DPATH must be set to the directory of the VRML\n";
    cout << "material appearance (ex: /usr/local/shared/kicad/kc3d/appearances)\n";

    return;
}

int main( int argc, char **argv )
{
    VRMLMAT bcolor, pcolor;

    int ic;
    string kc3ddir;

    while( ( ic = getopt( argc, argv, ":k:h" ) ) > 0 )
    {
        if( ic == ':' )
        {
            ERRBLURB;
            cerr << "missing option to '-" << (char) optopt << "'\n";
            printUsage();
            return -1;
        }

        if( ic == '?' )
        {
            ERRBLURB;
            cerr << "unrecognized option '-" << (char) optopt << "'\n";
            printUsage();
            return -1;
        }

        if( ic == 'h' )
        {
            printUsage();
            return 0;
        }

        if( ic == 'k' )
        {
            kc3ddir = optarg;
            break;
        }

    }

    if( kc3ddir.empty() )
    {
        char *evp = getenv( "KC3DPATH" );

        if( evp == NULL )
        {
            cout << "*** no KC3DPATH environment set and no path provided on command line\n";
            printUsage();
            return -1;
        }

        kc3ddir = evp;
    }

    string colorHsg;
    string colorPin;

    do
    {
        wxFileName chn( FROM_UTF8( kc3ddir.c_str() ), FROM_UTF8( "nylon_wht.mat" ) );
        wxFileName cpn( FROM_UTF8( kc3ddir.c_str() ), FROM_UTF8( "copper.mat" ) );

        colorHsg = TO_UTF8( chn.GetFullPath() );
        colorPin = TO_UTF8( cpn.GetFullPath() );
    } while(0);

    bcolor.Load( colorHsg );
    pcolor.Load( colorPin );

    PIN band;       // half of the U-shaped band to clamp the connector
    PIN thru[3];    // the parts of the PCB contacts

    TRANSFORM t0;
    t0.SetTranslation( 0, 0, -2.8 );
    PPARAMS pp;

    // band
    pp.r = 0.4;
    pp.bend = M_PI * 0.5;
    pp.nb   = 3;
    pp.w    = 0.4;
    pp.d    = 1.6;
    pp.bev  = 0.1;
    pp.h    = 9.1 - pp.r;
    pp.l    = 15.7 * 0.5 - pp.r;
    pp.tap  = -1;
    band.SetShape( true );
    band.Calc( pp, t0 );

    // narrow portion of pin
    t0.SetTranslation( 0, 0, -4.3 );
    pp.bev = 0.05;
    pp.h    = 4.8;
    pp.l    = -0.1;
    pp.r    = -0.1;
    pp.tap = 0.05;
    pp.dbltap = false;
    pp.stw  = 1.0;
    pp.std  = 0.94;
    thru[0].Calc( pp, t0 );

    // wide portion of pin
    t0.SetTranslation( 0, 0, 0.5 );
    pp.bev = 0.05;
    pp.d    = 4.6;  // 4.6/2 + 16 - 4.8 = 13.5mm spacing from pin to band
    pp.h    = 3.4;
    pp.l    = 6.0;
    pp.r    = 0.4;
    pp.tap = -0.1;
    thru[1].Calc( pp, t0 );

    // pin through casing
    t0.SetTranslation( 3.4, -0.15, 3.6 );
    pp.bev = -0.1;
    pp.w    = 6.0;
    pp.d    = 4.9;
    pp.h    = 0.6;
    pp.l    = -0.1;
    pp.r    = -0.1;
    pp.tap = -0.1;
    thru[2].Calc( pp, t0 );

    // back end of body
    PIN b1;
    t0.SetRotation( M_PI * 0.5, 1, 0, 0 );
    t0.SetTranslation( 0, -3.6, 3.15 );
    pp.bev = 0.2;
    pp.w    = 15.5;
    pp.d    = 6.3;
    pp.h    = 9.1;
    pp.r    = -0.1;
    pp.l    = -0.1;
    b1.Calc( pp, t0 );

    // middle portion of body
    PIN b2;
    t0.SetTranslation( 0, -12.7, 2.95 );
    pp.bev = 0.2;
    pp.w    = 15.3;
    pp.d    = 5.9;
    pp.h    = 1.6;
    pp.r    = -0.1;
    pp.l    = -0.1;
    b2.Calc( pp, t0 );

    // front portion of body
    PIN b3;
    t0.SetTranslation( 0, -14.3, 3.15 );
    pp.bev = 0.2;
    pp.w    = 15.5;
    pp.d    = 6.3;
    pp.h    = 4.0;
    pp.r    = -0.1;
    pp.l    = -0.1;
    b3.Calc( pp, t0 );

    // funnels to shroud rear and front ends
    FUNNEL shroud[3];
    shroud[0].SetShape( true, 0.2 );
    shroud[1].SetShape( true, 0.2 );
    shroud[2].SetShape( true, 0.2 );
    t0.SetTranslation( 0, -12.7, 3.15 );
    shroud[0].Calc( 15.5, 6.3, 14, 4.8, 0, 0, 9, t0 );
    t0.SetTranslation( 0, -18.3, 3.15 );
    shroud[2].Calc( 15.5, 6.3, 14.5, 5.3, 0, 0, 3, t0 );
    t0.SetRotation( -M_PI * 0.5, 1, 0, 0 );
    t0.SetTranslation( 0, -14.3, 3.15 );
    shroud[1].Calc( 15.5, 6.3, 13, 4.8, 0, 0, 3.0, t0 );
    // restore the previous rotation
    t0.SetRotation( M_PI * 0.5, 1, 0, 0 );

    // funnels to represent pins and their shrouds
    FUNNEL fn[4];
    fn[0].SetShape( true );
    fn[1].SetShape( true );
    fn[2].SetShape( true );
    fn[3].SetShape( true );
    t0.SetTranslation( -7.9 * 0.5, -17.6, 4.1 );
    fn[0].Calc( 3.4, 1.2, 3.0, 0.8, 0.2, 0, 13, t0 );
    t0.SetTranslation( 7.9 * 0.5, -17.6, 4.1 );
    fn[1].Calc( 2.0, 1.2, 1.6, 0.8, 0.2, 0, 13, t0 );
    t0.SetTranslation( -7.9 * 0.5, -17.8, 4.1 );
    fn[2].Calc( 3.8, 1.6, 3.4, 1.2, 0.2, 0, 2, t0 );
    t0.SetTranslation( 7.9 * 0.5, -17.8, 4.1 );
    fn[3].Calc( 2.4, 1.6, 2.0, 1.2, 0.2, 0, 2, t0 );

    // Holes to cover most of the front
    HOLE hole[2];
    t0.SetTranslation( -3.775, -17.8, 3.15 );
    hole[0].Calc( 7.55, 5.3, 3.8, 1.6, t0, true, -0.175, 0.95 );
    t0.SetTranslation( 3.775, -17.8, 3.15 );
    hole[1].Calc( 7.55, 5.3, 2.4, 1.6, t0, true, 0.175, 0.95 );

    // Pins to represent feedthrough (7x1.8mm)
    PIN feed[2];
    feed[0].SetShape( true );
    pp.w    = 6.6;
    pp.d    = 1.8;
    pp.r    = -0.1;
    pp.l    = -0.1;
    pp.h    = 1.0;
    pp.tap  = 0.25;
    pp.std  = 0.8;
    pp.stw  = 0.94;
    pp.bev  = 0.2;
    t0.SetTranslation( -3.4, -2.6, 4 );
    feed[0].Calc( pp, t0 );
    t0.SetTranslation( +3.4, -2.6, 4 );
    feed[1].Calc( pp, t0 );

    double s = 1.0 / 2.54;    // scale factor

    ofstream out;
    SetupVRML( "pcc-smp.wrl", out );
    SetupXForm( "TEST", out, 1 );
    t0.SetScale( s );
    t0.SetRotation( 0, 0, 0, 0 );
    t0.SetTranslation( 0, 0, 0 );
    // body
    b1.Build( true, false, t0, bcolor, false, out, 2 );
    b2.Build( false, false, t0, bcolor, false, out, 2 );
    b3.Build( false, false, t0, bcolor, false, out, 2 );
    // shroud to make body prettier
    shroud[0].Build( false, t0, bcolor, false, bcolor, false, out, 2 );
    shroud[1].Build( false, t0, bcolor, false, bcolor, false, out, 2 );
    shroud[2].Build( false, t0, bcolor, false, bcolor, false, out, 2 );
    // female pins and their shrouds
    fn[0].Build( true, t0, pcolor, false, pcolor, false, out, 2 );
    fn[1].Build( true, t0, pcolor, false, pcolor, false, out, 2 );
    fn[2].Build( true, t0, bcolor, false, bcolor, false, out, 2 );
    fn[3].Build( true, t0, bcolor, false, bcolor, false, out, 2 );
    // front surface
    hole[0].Build( true, t0, bcolor, false, out, 2 );
    hole[1].Build( true, t0, bcolor, false, out, 2 );
    // Thru pins
    t0.SetTranslation( -13.7 * 0.5 * s, 0, 0 );
    thru[0].Build( true, true, t0, pcolor, false, out, 2 );
    thru[1].Build( true, true, t0, pcolor, false, out, 2 );
    thru[2].Build( true, true, t0, pcolor, false, out, 2 );
    t0.SetTranslation( (13.7 * 0.5 - 6.8) * s * s, 0, 0 );
    thru[2].Build( true, true, t0, pcolor, false, out, 2 );
    // band
    t0.SetTranslation( -7.85 * s, -13.5 * s, 0 );
    band.Build( true, false, t0, pcolor, false, out, 2 );
    t0.SetTranslation( 0, 0, 0 );
    // plastic feedthroughs
    feed[0].Build( true, false, t0, bcolor, false, out, 2 );
    feed[1].Build( true, false, t0, bcolor, false, out, 2 );
    // Create the mirrored parts
    t0.SetTranslation( 13.7 * 0.5 * s, 0, 0 );
    t0.SetRotation( M_PI, 0, 0, 1 );
    thru[0].Build( true, true, t0, pcolor, false, out, 2 );
    thru[1].Build( true, true, t0, pcolor, false, out, 2 );
    t0.SetTranslation( 7.85 * s, -13.5 * s, 0 );
    band.Build( true, false, t0, pcolor, false, out, 2 );

    CloseXForm( out, 1 );
    out.close();
}
