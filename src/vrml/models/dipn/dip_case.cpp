/*
 *  file: dip_case.cpp
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
 *  These classes were created to support the
 *  programmatic creation of VRML models for
 *  use with KiCad.
 *
 */

#include <cmath>
#include <iomanip>
#include <fstream>
#include <iostream>

#include <vdefs.h>
#include <dip_case.h>

using namespace std;
using namespace KC3D;
using namespace KC3DDIP;

DIPCASE::DIPCASE()
{
    /* defaults are in inches for PDIP-24 */
    NW  = 0.06;
    NL  = 0.08;
    ND  = 0.02;
    BEV = 0.005;
    MID = 0.02;
    D   = 1.25;
    E1  = 0.26;
    S   = 0.01;
    A1  = 0.015;
    A2  = 0.13;
    valid = false;
}


int DIPCASE::SetCaseLength( double aCaseLength )
{
    valid = false;

    if( aCaseLength <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid value; must be > 0\n";
        return -1;
    }

    D = aCaseLength;
    return 0;
}


int DIPCASE::SetCaseWidth( double aCaseWidth )
{
    valid = false;

    if( aCaseWidth <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid value; must be > 0\n";
        return -1;
    }

    E1 = aCaseWidth;
    return 0;
}


int DIPCASE::SetCaseTaper( double aTaper )
{
    valid = false;

    if( aTaper < 0.0 )
    {
        ERRBLURB;
        cerr << "invalid value; must be >= 0\n";
        return -1;
    }

    S = aTaper;
    return 0;
}


int DIPCASE::SetBaseHeight( double aPCBOffset )
{
    valid = false;

    if( aPCBOffset < 0.0 )
    {
        ERRBLURB;
        cerr << "invalid value; must be >= 0\n";
        return -1;
    }

    A1 = aPCBOffset;
    return 0;
}


int DIPCASE::SetCaseDepth( double aCaseDepth )
{
    valid = false;

    if( aCaseDepth <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid value; must be > 0\n";
        return -1;
    }

    A2 = aCaseDepth;
    return 0;
}


int DIPCASE::SetNotchWidth( double aNotchWidth )
{
    valid = false;

    if( aNotchWidth <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid value; must be > 0\n";
        return -1;
    }

    NW = aNotchWidth;
    return 0;
}


int DIPCASE::SetNotchDepth( double aNotchDepth )
{
    valid = false;

    if( aNotchDepth <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid value; must be > 0\n";
        return -1;
    }

    ND = aNotchDepth;
    return 0;
}


int DIPCASE::SetCaseBevel( double aBevel )
{
    valid = false;

    if( aBevel <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid value; must be > 0\n";
        return -1;
    }

    BEV = aBevel;
    return 0;
}


int DIPCASE::SetCaseMidHeight( double aCaseMidZ )
{
    valid = false;

    if( aCaseMidZ <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid value; must be > 0\n";
        return -1;
    }

    MID = aCaseMidZ;
    return 0;
}


int DIPCASE::SetNotchLength( double aNotchLength )
{
    valid = false;

    if( aNotchLength <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid value; must be > 0\n";
        return -1;
    }

    NL = aNotchLength;
    return 0;
}


int DIPCASE::Calc( void )
{
    int i;
    double height;
    double slope;
    double off;
    double ndia;

    /* TODO: check constraints */

    ndia = NW / 2.0;
    slope = S / ( (A2 - MID) / 2 - BEV );

    /* L1, points 0..4 */
    for( i = 0; i < 4; ++i )
        p[i].z = A1;

    p[0].x  = -D / 2 + S + BEV;
    p[0].y  = -E1 / 2 + S + BEV;
    p[1].x  = -p[0].x;
    p[1].y  = p[0].y;
    p[2].x  = p[1].x;
    p[2].y  = -p[1].y;
    p[3].x  = p[0].x;
    p[3].y  = p[2].y;
    /* L2, points 4..11 */
    height = A1 + BEV;

    for( i = 4; i < 12; ++i )
        p[i].z = height;

    p[4].x  = p[11].x = -D / 2 + S;
    p[5].x  = p[10].x = p[4].x + BEV;
    p[6].x  = p[9].x = -p[5].x;
    p[7].x  = p[8].x = -p[4].x;
    p[4].y  = p[7].y = -E1 / 2 + S + BEV;
    p[5].y  = p[6].y = p[4].y - BEV;
    p[8].y  = p[11].y = -p[4].y;
    p[9].y  = p[10].y = -p[5].y;
    /* L3, points 12..19 */
    height = A1 + A2 / 2 - MID / 2;

    for( i = 12; i < 20; ++i )
        p[i].z = height;

    p[12].x = -D / 2;
    p[12].y = -E1 / 2 + BEV;
    p[13].x = -D / 2 + BEV;
    p[13].y = p[12].y - BEV;
    p[14].x = -p[13].x;
    p[14].y = p[13].y;
    p[15].x = -p[12].x;
    p[15].y = p[12].y;
    p[16].x = p[15].x;
    p[16].y = -p[15].y;
    p[17].x = p[14].x;
    p[17].y = -p[14].y;
    p[18].x = p[13].x;
    p[18].y = p[17].y;
    p[19].x = p[12].x;
    p[19].y = p[16].y;
    /* L4, points 20 .. 27 */
    height += MID;

    for( i = 20; i < 28; ++i )
    {
        p[i].z  = height;
        p[i].x  = p[i - 8].x;
        p[i].y  = p[i - 8].y;
    }

    /* L5, points 28 .. 29 */
    height  = A1 + A2 - ND - BEV - BEV;
    p[28].z = p[29].z = height;
    off = ( height - (A1 + (A2 + MID) / 2) ) * slope;
    p[28].x = p[29].x = p[20].x + off;
    p[28].y = ndia + BEV;
    p[29].y = -p[28].y;
    /* L6, points 30..39 */
    height += BEV;

    for( i = 30; i < 40; ++i )
        p[i].z = height;

    off = ( height - (A1 + (A2 + MID) / 2) ) * slope;
    p[30].x = p[39].x = p[20].x + off + BEV;
    p[31].x = p[38].x = -D / 2 + NL - ndia;
    p[32].x = p[37].x = p[31].x + ndia * cos( 3.0 / 8.0 * M_PI );
    p[33].x = p[36].x = p[31].x + ndia * cos( M_PI / 4.0 );
    p[34].x = p[35].x = p[31].x + ndia * cos( M_PI / 8.0 );
    p[30].y = p[31].y = ndia;
    p[32].y = ndia * sin( 3.0 / 8.0 * M_PI );
    p[33].y = ndia * sin( M_PI / 4.0 );
    p[34].y = ndia * sin( M_PI / 8.0 );
    p[35].y = -p[34].y;
    p[36].y = -p[33].y;
    p[37].y = -p[32].y;
    p[38].y = p[39].y = -p[31].y;
    /* L7, points 40 .. 51 (notch), 52 .. 59 (rectangle) */
    height = A1 + A2 - BEV;

    for( i = 40; i < 60; ++i )
        p[i].z = height;

    p[40].x = p[51].x = -D / 2 + S;
    p[41].x = p[50].x = p[51].x + BEV;

    for( i = 42; i < 50; ++i )
    {
        p[i].x  = p[i - 11].x;
        p[i].y  = p[i - 11].y;
    }

    p[41].y = p[30].y;
    p[50].y = p[39].y;
    p[40].y = p[41].y + BEV;
    p[51].y = -p[40].y;

    for( i = 52; i < 60; ++i )
    {
        p[i].x  = p[i - 48].x;
        p[i].y  = p[i - 48].y;
    }

    /* L8, 60 .. 75 */
    height = A1 + A2;

    for( i = 60; i < 76; ++i )
        p[i].z = height;

    p[60].x = p[69].x = -D / 2 + S + BEV;
    p[61].x = p[68].x = -D / 2 + NL - ndia;
    ndia += BEV;
    p[62].x = p[67].x = p[61].x + ndia * cos( 3.0 / 8.0 * M_PI );
    p[63].x = p[66].x = p[61].x + ndia * cos( M_PI / 4.0 );
    p[64].x = p[65].x = p[61].x + ndia * cos( M_PI / 8.0 );
    p[60].y = p[61].y = ndia;
    p[62].y = ndia * sin( 3.0 / 8.0 * M_PI );
    p[63].y = ndia * sin( M_PI / 4.0 );
    p[64].y = ndia * sin( M_PI / 8.0 );
    p[69].y = p[68].y = -p[60].y;
    p[67].y = -p[62].y;
    p[66].y = -p[63].y;
    p[65].y = -p[64].y;
    p[70].x = p[75].x = -D / 2 + S + BEV;
    p[71].x = p[74].x = -D / 2 + NL + NW;
    p[72].x = p[73].x = -p[70].x;
    p[70].y = p[71].y = p[72].y = -E1 / 2 + S + BEV;
    p[73].y = p[74].y = p[75].y = -p[70].y;
    /* L9, 76 . .79 = the internal baffle */
    p[76].z = p[77].z = p[78].z = p[79].z = A1 + A2 / 2 + MID / 2;
    p[76].x = p[13].x;
    p[76].y = p[13].y + 0.002;
    p[77].x = p[14].x;
    p[77].y = p[14].y + 0.002;
    p[78].x = p[17].x;
    p[78].y = p[17].y - 0.002;
    p[79].x = p[18].x;
    p[79].y = p[18].y - 0.002;
    valid = true;
    return 0;
}


int DIPCASE::WriteCoord( TRANSFORM& aTransform, std::ofstream& aVRMLFile, int aTabDepth )
{
    int i;
    QUAT loc;

    if( !valid )
    {
        ERRBLURB;
        cerr << "invoked without prior successful call to calc()\n";
        return -1;
    }

    if( aTabDepth < 0 )
        aTabDepth = 0;

    if( aTabDepth > 4 )
        aTabDepth = 4;

    string fmt( 4 * aTabDepth, ' ' );

    aVRMLFile << fmt << "coord Coordinate { point [\n";
    aVRMLFile << fmt << "   ";

    for( i = 0; i < CASE_NP - 1; ++i )
    {
        loc = p[i];
        aTransform.Transform( loc );
        aVRMLFile << setprecision( 8 ) << " " << loc.x << " " << loc.y << " " << loc.z << ",";

        if( !( (i + 1) % 6 ) )
            aVRMLFile << "\n" << fmt << "   ";
    }

    loc = p[i];
    aTransform.Transform( loc );
    aVRMLFile << setprecision( 8 ) << " " << loc.x << " " << loc.y << " " << loc.z << " ]\n";
    aVRMLFile << fmt << "}\n";

    return aVRMLFile.good() ? 0 : -1;
}


int DIPCASE::WriteFacets( std::ofstream& aVRMLFile, int aTabDepth )
{
    if( aTabDepth < 0 )
        aTabDepth = 0;

    if( aTabDepth > 4 )
        aTabDepth = 4;

    string fmt( 4 * aTabDepth, ' ' );

    aVRMLFile << fmt << "coordIndex [\n";
    /* case, bottom */
    aVRMLFile << fmt << "    0, 3, 2, 1, -1, 0, 5, 4, -1, 0, 1, 6, 5, -1, 1, 7, 6, -1,\n";
    aVRMLFile << fmt <<
    "    1, 2, 8, 7, -1, 2, 9, 8, -1, 2, 3, 10, 9, -1, 3, 11, 10, -1, 3, 0, 4, 11, -1,\n";
    /* case, lower bevel */
    aVRMLFile << fmt << "    4, 12, 19, 11, -1, 5, 13, 12, 4, -1, 6, 14, 13, 5, -1, 7, 15, 14, 6, -1,\n";
    aVRMLFile << fmt << "    8, 16, 15, 7, -1, 9, 17, 16, 8, -1, 10, 18, 17, 9, -1, 11, 19, 18, 10, -1,\n";
    /* case, middle */
    aVRMLFile << fmt <<
    "    13, 21, 20, 12, -1, 14, 22, 21, 13, -1, 15, 23, 22, 14, -1, 16, 24, 23, 15, -1,\n";
    aVRMLFile << fmt <<
    "    17, 25, 24, 16, -1, 18, 26, 25, 17, -1, 19, 27, 26, 18, -1, 12, 20, 27, 19, -1,\n";
    /* case, upper bevel except front */
    aVRMLFile << fmt <<
    "    21, 53, 52, 20, -1, 22, 54, 53, 21, -1, 23, 55, 54, 22, -1, 24, 56, 55, 23, -1,\n";
    aVRMLFile << fmt << "    25, 57, 56, 24, -1, 26, 58, 57, 25, -1, 27, 59, 58, 26, -1,\n";
    /* case, upper bevel front */
    aVRMLFile << fmt << "    27, 28, 40, 59, -1, 20, 52, 51, 29, -1, 20, 29, 28, 27, -1,\n";
    /* case, notch, front bevel */
    aVRMLFile << fmt << "    29, 51, 50, 39, -1, 28, 30, 41, 40, -1, 28, 29, 39, 30, -1,\n";
    /* case, notch, floor */
    aVRMLFile << fmt << "    39, 38, 37, 36, 35, 34, 33, 32, 31, 30, -1,\n";
    /* case, notch, wall */
    aVRMLFile << fmt <<
    "    39, 50, 49, 38, -1, 38, 49, 48, 37, -1, 37, 48, 47, 36, -1, 36, 47, 46, 35, -1, 35, 46, 45, 34, -1,\n";
    aVRMLFile << fmt <<
    "    34, 45, 44, 33, -1, 33, 44, 43, 32, -1, 32, 43, 42, 31, -1, 31, 42, 41, 30, -1,\n";
    /* case, top bevel except front */
    aVRMLFile << fmt << "    53, 70, 52, -1, 53, 54, 72, 70, -1, 54, 55, 72, -1, 55, 56, 73, 72, -1,\n";
    aVRMLFile << fmt << "    56, 57, 73, -1, 57, 58, 75, 73, -1, 58, 59, 75, -1,\n";
    /* case, notch, top bevel */
    aVRMLFile << fmt <<
    "    59, 40, 60, 75, -1, 40, 41, 60, -1, 41, 42, 61, 60, -1, 42, 43, 62, 61, -1,\n";
    aVRMLFile << fmt <<
    "    43, 44, 63, 62, -1, 44, 45, 64, 63, -1, 45, 46, 65, 64, -1, 46, 47, 66, 65, -1, 47, 48, 67, 66, -1,\n";
    aVRMLFile << fmt <<
    "    48, 49, 68, 67, -1, 49, 50, 69, 68, -1, 50, 51, 69, -1, 51, 52, 70, 69, -1,\n";
    /* case, top */
    aVRMLFile << fmt << "    68, 69, 70, 71, -1, 67, 68, 71, -1, 66, 67, 71, -1, 65, 66, 71, -1,\n";
    aVRMLFile << fmt << "    74, 75, 60, 61, -1, 74, 61, 62, -1, 74, 62, 63, -1, 74, 63, 64, -1,\n";
    aVRMLFile << fmt << "    64, 65, 71, 74, -1, 71, 72, 73, 74, -1, 76, 77, 78, 79, -1\n" << fmt << "]\n";
    return aVRMLFile.good() ? 0 : -1;
}


int DIPCASE::SetParams( double aCaseLength, double aCaseWidth, double aPCBOffset,
                        double aCaseDepth, double aNotchWidth, double aNotchDepth,
                        double aNotchLength, double aCaseMidZ, double aBevel, double aTaper )
{
    valid = false;

    if( aCaseLength <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid dimension (aCaseLength); must be > 0\n";
        return -1;
    }

    D = aCaseLength;

    if( aCaseMidZ <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid dimension (aCaseMidZ); must be > 0\n";
        return -1;
    }

    MID = aCaseMidZ;

    if( aCaseWidth <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid dimension (aCaseWidth); must be > 0\n";
        return -1;
    }

    E1 = aCaseWidth;

    if( aCaseDepth <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid dimension (aCaseDepth); must be > 0\n";
        return -1;
    }

    A2 = aCaseDepth;

    if( aPCBOffset < 0.0 )
    {
        ERRBLURB;
        cerr << "invalid dimension (aPCBOffset); must be >= 0\n";
        return -1;
    }

    A1 = aPCBOffset;

    if( aNotchWidth <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid dimension (aNotchWidth); must be > 0\n";
        return -1;
    }

    NW = aNotchWidth;

    if( aNotchDepth <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid dimension (aNotchDepth); must be > 0\n";
        return -1;
    }

    if( aNotchDepth >= (aCaseDepth - aCaseMidZ) * 0.5 )
    {
        ERRBLURB;
        cerr << "invalid dimension (aNotchDepth); must be < (aCaseDepth - aCaseMidZ)/2\n";
        return -1;
    }

    ND = aNotchDepth;

    if( aNotchLength <= aNotchWidth )
    {
        ERRBLURB;
        cerr << "invalid dimension (aNotchLength); must be > aNotchWidth\n";
        return -1;
    }

    NL = aNotchLength;

    if( aBevel <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid dimension (aBevel); must be > 0\n";
        return -1;
    }

    BEV = aBevel;

    if( aTaper < 0.0 )
    {
        ERRBLURB;
        cerr << "invalid dimension (aTaper); must be >= 0\n";
        return -1;
    }

    S = aTaper;

    return 0;
}
