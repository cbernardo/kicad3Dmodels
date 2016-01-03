/*
 *      file: makeMFR.cpp
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
 *  This program reads a control file to produce a series of tubular thru-hole
 *  resistor models based on the following parameters:
 *
 *  scale   : world scale to control final VRML output scale
 *  shift   : X-axis shift to control centering of the output model
 *  L       : length of resistor body
 *  D       : diameter of resistor body
 *  d       : diameter of wire
 *  p       : lead pitch
 *  horiz   : orientation; true or 1 if horizontal, false or 0 if vertical
 *  endshape: end style; 'C'ap, 'R'ound, 'B'ulge (default)
 *  wsides  : number of sides to the wire (default 16)
 *  bsides  : number of sides in a 90-deg bend (default 6)
 *  rsides  : number of sides on the resistor (default 48)
 *  c0..c13 : path to color file for color bands 0..9, Gold, Silver, Body Color, Wire Color
 *  values  : path to file containing filename : color code pairs
 *
 *  Example of a values file entry for a horizontal 12.1K resistor, 1% (F), no Tcoeff code:
 *  LR0204_12K1_F_BH.wrl : 11X22X11X22XXX111
 *
 *  The format for the file name should be as follows:
 *  [Part Series]_[VALUE]_[Tol. Code][Tcoeff Code][End Style][Orientation].wrl
 *  If the tolerance is not given (20%), the character shall be '-'; likewise
 *  if the temperature coefficient code is not given, '-' shall be used.
 *
 *  The color code string affects the appearance in the following way:
 *  the space between the two end caps of the resistor is divided into
 *  a bands of equal width based on the number of characters in the color
 *  code string. Each band is painted the given color - 0..9 represent the
 *  value codes for 0..9, G represents GOLD, S represents SILVER, and
 *  X represents the resistor body color.  This scheme gives the user
 *  quite a bit of control over the overall width, spacing, and position
 *  of each colored band.
 *
 */

/*
 *  Suggestion for the directory structure:
 *
 *  mcad/vrml/thru/[IEC Series]/[Part Series]
 *
 *  IEC Series  : E192, E96, E24, E48, E12, E6, E3
 *  Part Series : depends on manufacturer, for example TT
 *      have E196 series named LR0202, LR1, LR2, LR100, LR200
 */

#include <unistd.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <wx/filename.h>

#include <macros.h>
#include <vdefs.h>
#include <polygon.h>
#include <vcom.h>
#include <transform.h>
#include <vrmlmat.h>
#include <resistor.h>
#include <keyvalparser.h>

extern char*    optarg;
extern int      optopt;

using namespace std;
using namespace KC3D;
using namespace KC3DRESISTOR;

void printUsage( void )
{
    cout << "Usage: makeMFR [-h] [-k appearances_path] -p paramFile -o outputList\n";
    return;
}


int GetParams( const std::string& pfile, RPARAMS& rp, const std::string& matdir );

int main( int argc, char** argv )
{
    KEYVAL_PARSER outputs;
    RPARAMS params;
    string  paramFile;
    string  outFile;
    RESISTOR res;
    string kc3ddir;

    int ich;

    while( ( ich = getopt( argc, argv, ":hp:o:k:" ) ) > 0 )
    {
        if( ich == ':' )
        {
            ERRBLURB;
            cerr << "missing option to '-" << (char) optopt << "'\n";
            printUsage();
            return -1;
        }

        if( ich == '?' )
        {
            ERRBLURB;
            cerr << "unrecognized option '-" << (char) optopt << "'\n";
            printUsage();
            return -1;
        }

        if( ich == 'h' )
        {
            printUsage();
            return 0;
        }

        if( ich == 'p' )
        {
            paramFile.assign( optarg );
            cerr << "param file: " << paramFile << "\n";
            continue;
        }

        if( ich == 'o' )
        {
            outFile.assign( optarg );
            cerr << "output list: " << outFile << "\n";
            continue;
        }

        if( ich == 'k' )
        {
            kc3ddir = optarg;
            continue;
        }

    }   // optarg parsing

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

    if( paramFile.empty() )
    {
        ERRBLURB;
        cerr << "no parameters file specified\n";
        printUsage();
        return -1;
    }

    if( outFile.empty() )
    {
        ERRBLURB;
        cerr << "no output file list specified\n";
        printUsage();
        return -1;
    }

    if( GetParams( paramFile, params, kc3ddir ) )
    {
        ERRBLURB;
        cerr << "unable to read parameters from file: '" << paramFile << "'\n";
        return -1;
    }

    if( outputs.LoadKeys( outFile ) <= 0 )
    {
        ERRBLURB;
        cerr << "unable to read output list from file: '" << paramFile << "'\n";
        return -1;
    }

    map<string, string>::const_iterator lstart = outputs.GetKeys().begin();
    map<string, string>::const_iterator lend = outputs.GetKeys().end();

    int i = 0;

    while( lstart != lend )
    {
        ++i;

        if( res.Create( params, lstart->second, lstart->first ) )
        {
            ERRBLURB;
            cerr << "bailing out due to error\n";
            cerr << "Problems at item #" << i << "\n";
            return -1;
        }

        ++lstart;
    }

    return 0;
}


int GetParams( const std::string& pfile, RPARAMS& rp, const std::string& matdir )
{
    KEYVAL_PARSER p;

    if( p.LoadKeys( pfile ) <= 0 )
        return 0;

    const map<string, string>& k = p.GetKeys();
    map<string, string>::const_iterator kp;
    map<string, string>::const_iterator kend = k.end();

    istringstream os;
    kp = k.find( "scale" );

    if( kp != kend )
    {
        os.clear();
        os.str( kp->second );
        os >> rp.scale;
    }

    kp = k.find( "shift" );

    if( kp != kend )
    {
        os.clear();
        os.str( kp->second );
        os >> rp.shift;
    }

    kp = k.find( "L" );

    if( kp != kend )
    {
        os.clear();
        os.str( kp->second );
        os >> rp.L;
    }

    kp = k.find( "D" );

    if( kp != kend )
    {
        os.clear();
        os.str( kp->second );
        os >> rp.D;
    }

    kp = k.find( "d" );

    if( kp != kend )
    {
        os.clear();
        os.str( kp->second );
        os >> rp.d;
    }

    kp = k.find( "p" );

    if( kp != kend )
    {
        os.clear();
        os.str( kp->second );
        os >> rp.p;
    }

    kp = k.find( "horiz" );

    if( kp != kend )
    {
        if( ( !kp->second.compare( "false" ) )
            || ( !kp->second.compare( "0" ) ) )
        {
            rp.horiz = false;
        }
        else
        {
            rp.horiz = true;
        }
    }

    kp = k.find( "endstyle" );

    if( kp != kend )
    {
        os.clear();
        os.str( kp->second );
        os >> rp.endshape;
    }

    if( rp.endshape == 'B' )
    {
        kp = k.find( "bcap" );

        if( kp != kend )
        {
            if( ( !kp->second.compare( "false" ) )
                || ( !kp->second.compare( "0" ) ) )
            {
                rp.bcap = false;
            }
            else
            {
                rp.bcap = true;
            }
        }
    }
    else
    {
        rp.bcap = false;
    }

    kp = k.find( "wsides" );

    if( kp != kend )
    {
        os.clear();
        os.str( kp->second );
        os >> rp.wsides;
    }

    kp = k.find( "bsides" );

    if( kp != kend )
    {
        os.clear();
        os.str( kp->second );
        os >> rp.bsides;
    }

    kp = k.find( "rsides" );

    if( kp != kend )
    {
        os.clear();
        os.str( kp->second );
        os >> rp.rsides;
    }

    kp = k.find( "pwrsuf" );

    if( kp != kend )
    {
        rp.pwrsuf.assign( kp->second );
    }

    kp = k.find( "spcsuf" );

    if( kp != kend )
    {
        rp.spcsuf.assign( kp->second );
    }

    int i;
    ostringstream oss;

    for( i = 0; i < 14; ++i )
    {
        oss.str( "" );
        oss << "color" << i;
        kp = k.find( oss.str() );

        if( kp != kend )
        {
            wxFileName cfn( FROM_UTF8( matdir.c_str() ), FROM_UTF8( kp->second.c_str() ) );

            if( rp.colors[i].Load( TO_UTF8( cfn.GetFullPath() ) ) )
            {
                ERRBLURB;
                cerr << "cannot load color #" << i << " ('";
                cerr << TO_UTF8( cfn.GetFullPath() ) << "')\n";
                return -1;
            }
        }
        else
        {
            ERRBLURB;
            cerr << "no specification for color #" << i << "\n";
            return -1;
        }
    }

    return 0;
}
