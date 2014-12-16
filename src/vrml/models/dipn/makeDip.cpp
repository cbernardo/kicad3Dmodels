/*
 *  file: makeDip.cpp
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
 *  Notes:
 *  This is a quick and dirty test of the dip class for creating DIPn
 *  VRML models for use with KiCad. Other sized DIPn packages can be
 *  created via the python scripting facility; the scripting allows
 *  substantial customization including the omission of specified pins.
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <string>
#include <wx/filename.h>

#include <macros.h>
#include <vdefs.h>
#include <dip.h>
#include <transform.h>
#include <k3dconfig.h>

using namespace std;
using namespace KC3DDIP;

extern char* optarg;
extern int optopt;

// 0.3" DIP
int mkI300( const std::string& matdir );

// 0.6" DIP
int mkI600( const std::string& matdir );

void printUsage( void )
{
    cout << "Usage: makeDip [-k appearances_path]\n";
    cout << "When invoked without a path specification, the environment\n";
    cout << "variable KC3DPATH must be set to the directory of the VRML\n";
    cout << "material appearance (ex: /usr/local/shared/kicad/kc3d/appearances)\n";

    return;
}

int main( int argc, char **argv )
{
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

    if( mkI300( kc3ddir ) )
    {
        fprintf( stderr, "problems creating 0.3-inch DILs\n" );
        return -1;
    }

    if( mkI600( kc3ddir ) )
    {
        fprintf( stderr, "problems creating 0.3-inch DILs\n" );
        return -1;
    }

    return 0;
}


int mkI300( const std::string& matdir )
{
    DIP dil;
    char    name[256];
    int     i;
    DIPPARAMS dp;

    string colorHsg;
    string colorPin;

    do
    {
        wxFileName chn( FROM_UTF8( matdir.c_str() ), FROM_UTF8( "ceram_gry.mat" ) );
        wxFileName cpn( FROM_UTF8( matdir.c_str() ), FROM_UTF8( "tin.mat" ) );

        colorHsg = TO_UTF8( chn.GetFullPath() );
        colorPin = TO_UTF8( cpn.GetFullPath() );
    } while(0);

    dil.SetCaseColor( colorHsg );
    dil.SetPinColor( colorPin );
    dil.SetParams( dp );

    for( i = 4; i <= 28; i += 2 )
    {
        sprintf( name, "dil%d_i300.wrl", i );
        dil.SetPins( i );

        if( dil.Build( name ) )
            return -1;
    }

    /*
     *  // demo of the pin skipping function
     *  int j;
     *  for(i = 4; i <= 28; i+=2)
     *  {
     *  sprintf(name, "dil%d_i300.wrl", i);
     *  dil.SetPins(i);
     *  for (j = 3; j <= (i/2 -2); ++j)
     *  {
     *     dil.SetPin(j, false);
     *     dil.SetPin(i - j +1, false);
     *  }
     *  if (dil.Build(name)) return -1;
     *  }
     */
    return 0;
}


int mkI600( const std::string& matdir )
{
    DIP dil;
    char    name[256];
    int     i;
    DIPPARAMS dp;

    string colorHsg;
    string colorPin;

    do
    {
        wxFileName chn( FROM_UTF8( matdir.c_str() ), FROM_UTF8( "ceram_gry.mat" ) );
        wxFileName cpn( FROM_UTF8( matdir.c_str() ), FROM_UTF8( "tin.mat" ) );

        colorHsg = TO_UTF8( chn.GetFullPath() );
        colorPin = TO_UTF8( cpn.GetFullPath() );
    } while(0);

    dil.SetCaseColor( colorHsg );
    dil.SetPinColor( colorPin );

    // parameters suited to 40-pin PDIP for PIC16F7X
    dp.A1   = 0.025;
    dp.A2   = 0.15;
    dp.DW   = 0.058;
    dp.E    = 0.6;
    dp.E1   = 0.545;
    dil.SetParams( dp );

    for( i = 40; i <= 40; i += 2 )
    {
        sprintf( name, "dil%d_i600.wrl", i );
        dil.SetPins( i );

        if( dil.Build( name ) )
            return -1;
    }

    return 0;
}
