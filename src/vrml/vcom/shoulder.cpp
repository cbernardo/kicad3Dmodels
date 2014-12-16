/*
 *      file: shoulder.cpp
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
 *
 *      This class represents a shoulder such as the one found on the
 *      bottom of a SIL header which creates a small gap between the
 *      component and the board.
 *
 */

#include <iostream>
#include <fstream>
#include <cmath>

#include <shoulder.h>
#include <vdefs.h>
#include <vcom.h>
#include <transform.h>

using namespace std;
using namespace KC3D;

SHOULDER::SHOULDER()
{
    valid = false;
    np = 0;

    return;
}


SHOULDER::~SHOULDER()
{
    valid = false;

    return;
}


int SHOULDER::Calc( double aLength, double aHeight, double aWidth, double aTaper,
                    double aRadius, TRANSFORM& aTransform )
{
    valid = false;

    // validate the parameters
    if( aLength <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid length (<= 0)\n";
        return -1;
    }

    if( aHeight <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid height (<= 0)\n";
        return -1;
    }

    if( aWidth <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid width (<= 0)\n";
        return -1;
    }

    bool    has_t   = false;
    bool    has_r   = false;

    if( aTaper > 0.0 )
    {
        has_t = true;

        if( aTaper > 0.4 * M_PI )
        {
            ERRBLURB;
            cerr << "invalid taper (> 0.4*M_PI)\n";
            return -1;
        }

        if( aLength <= 2.0 * aHeight * tan( aTaper ) )
        {
            ERRBLURB;
            cerr << "invalid taper; angle is too steep or length is too small\n";
            return -1;
        }
    }

    np = 4;

    if( aRadius > 0.0 )
    {
        has_r = true;
        np = 7;

        if( aRadius >= aWidth )
        {
            ERRBLURB;
            cerr << "invalid radius (>= width)\n";
            return -1;
        }

        if( aRadius >= aHeight )
        {
            ERRBLURB;
            cerr << "invalid radius (>= height)\n";
            return -1;
        }
    }

    int idx = 0;
    double l2 = aLength / 2.0;

    // Point 0
    p0[0][idx]  = -l2;
    p0[1][idx]  = 0.0;
    p0[2][idx]  = aHeight;
    ++idx;

    // Point 1
    p0[0][idx]  = -l2;
    p0[1][idx]  = 0.0;
    p0[2][idx]  = 0.0;

    if( has_t )
        p0[0][idx] += aHeight * tan( aTaper );

    ++idx;

    int i;

    if( has_r )
    {
        double  da = 0.5 * M_PI / 3.0;
        double  ang;

        for( i = 0; i < 4; ++i )
        {
            ang = i * da;
            p0[0][idx]  = -l2;
            p0[1][idx]  = -aWidth + aRadius * ( 1 - sin( ang ) );
            p0[2][idx]  = aRadius * ( 1 - cos( ang ) );

            if( has_t )
                p0[0][idx] += (aHeight - p0[2][idx]) * tan( aTaper );

            ++idx;
        }
    }
    else
    {
        p0[0][idx]  = -l2;
        p0[1][idx]  = -aWidth;
        p0[2][idx]  = 0.0;

        if( has_t )
            p0[0][idx] += aHeight * tan( aTaper );

        ++idx;
    }

    // Last point
    p0[0][idx]  = -l2;
    p0[1][idx]  = -aWidth;
    p0[2][idx]  = aHeight;

    // mirror the part
    for( idx = 0; idx < np; ++idx )
    {
        p1[0][idx]  = -p0[0][idx];
        p1[1][idx]  = p0[1][idx];
        p1[2][idx]  = p0[2][idx];
    }

    aTransform.Transform( p0[0], p0[1], p0[2], np );
    aTransform.Transform( p1[0], p1[1], p1[2], np );

    valid = true;

    return 0;
}


int SHOULDER::Build( TRANSFORM& aTransform, VRMLMAT& aMaterial, bool reuseMaterial,
                     std::ofstream& aVRMLFile, int aTabDepth )
{
    if( !valid )
    {
        ERRBLURB;
        cerr << "invoked with no prior successful call to Calc()\n";
        return -1;
    }

    double tp0[3][14];

    int i;

    for( i = 0; i < np; ++i )
    {
        tp0[0][i]   = p0[0][i];
        tp0[1][i]   = p0[1][i];
        tp0[2][i]   = p0[2][i];
        tp0[0][i + np]  = p1[0][i];
        tp0[1][i + np]  = p1[1][i];
        tp0[2][i + np]  = p1[2][i];
    }

    aTransform.Transform( tp0[0], tp0[1], tp0[2], np * 2 );

    // CAP0
    // set up VRML Shape
    SetupShape( aMaterial, reuseMaterial, aVRMLFile, aTabDepth );
    // enumerate vertices
    WriteCoord( tp0[0], tp0[1], tp0[2], np, aVRMLFile, aTabDepth + 1 );
    // enumerate facets
    SetupCoordIndex( aVRMLFile, aTabDepth + 1 );
    string fmt( (aTabDepth + 1) * 4, ' ' );
    aVRMLFile << fmt << "    ";

    for( i = 0; i < np; ++i )
    {
        aVRMLFile << i << ",";
    }

    aVRMLFile << "0,-1\n";
    CloseCoordIndex( aVRMLFile, aTabDepth + 1 );
    CloseShape( aVRMLFile, aTabDepth );

    // CAP1
    // set up VRML Shape
    SetupShape( aMaterial, true, aVRMLFile, aTabDepth );
    // enumerate vertices
    WriteCoord( &tp0[0][np], &tp0[1][np], &tp0[2][np], np, aVRMLFile, aTabDepth + 1 );
    // enumerate facets
    SetupCoordIndex( aVRMLFile, aTabDepth + 1 );
    aVRMLFile << fmt << "    ";

    for( i = np - 1; i > 0; --i )
    {
        aVRMLFile << i << ",";
    }

    aVRMLFile << "0," << np - 1 << ",-1\n";
    CloseCoordIndex( aVRMLFile, aTabDepth + 1 );
    CloseShape( aVRMLFile, aTabDepth );

    // Body
    // set up VRML Shape
    SetupShape( aMaterial, true, aVRMLFile, aTabDepth );
    // enumerate vertices
    WriteCoord( tp0[0], tp0[1], tp0[2], np * 2, aVRMLFile, aTabDepth + 1 );
    // enumerate facets
    SetupCoordIndex( aVRMLFile, aTabDepth + 1 );
    aVRMLFile << fmt << "    ";

    for( i = 0; i < np - 2; ++i )
    {
        aVRMLFile << i + 1 << "," << i << "," <<
            i + np << "," << i + np + 1 << ",-1,";

        if( !( (i + 1) % 18 ) )
            aVRMLFile << "\n" << fmt << "    ";
    }

    aVRMLFile << np - 1 << "," << np - 2 << "," <<
        np + np - 2 << "," << np + np - 1 << ",-1\n";
    CloseCoordIndex( aVRMLFile, aTabDepth + 1 );

    return CloseShape( aVRMLFile, aTabDepth );
}
