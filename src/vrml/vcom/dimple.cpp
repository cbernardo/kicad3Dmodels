/*
 *      file: dimple.cpp
 *
 *      Copyright 2012-2014 Dr. Cirilo Bernardo (cjh.bernardo@gmail.com)
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
 *      This class creates a faceted bump based on a given Circle and bump depth.
 *
 */

#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

#include <vdefs.h>
#include <vcom.h>
#include <transform.h>
#include <dimple.h>


using namespace std;
using namespace KC3D;


DIMPLE::DIMPLE()
{
    paramSet = false;
    refAngle = 0.0;
    dAngle = 0.0;
    factor = 0.0;
    sides = 0;
    sections = 0;
    depth = 0;

    return;
}

bool DIMPLE::SetParams( double aRadius, double aDepth, int aNSides, int aNSections )
{
    paramSet = false;

    if( aDepth < 0.0 )
        factor = -1.0;
    else
        factor = 1.0;

    if( aRadius <= 0.0 )
    {
        ERRBLURB << "invalid radius\n";
        return false;
    }

    if( aDepth < 0.0000001 && aDepth > -0.0000001 )
    {
        ERRBLURB << "invalid aDepth (0)\n";
        return false;
    }

    if( aDepth * factor > aRadius * 50.0 )
    {
        ERRBLURB << "aDepth exceeds aRadius * 50\n";
        return false;
    }

    if( aNSides < 3 )
    {
        ERRBLURB << "aNSides < 3\n";
        return false;
    }

    if( aNSections < 2 )
    {
        ERRBLURB << "aNSections < 2\n";
        return false;
    }

    bump.SetNVertices( aNSides );
    sections = aNSections;
    sides = aNSides;
    KC3D::TRANSFORM t0;
    bump.Calc( aRadius * 2.0, aRadius * 2.0, t0 );

    double R;   // radius of the sphere
    R = ( aRadius * aRadius + aDepth * aDepth ) / ( 2.0 * factor * aDepth );

    if( aDepth * factor > aRadius )
        refAngle = M_PI / 2.0 + atan2( factor * aDepth - R, aRadius );
    else
        refAngle = M_PI / 2.0 - atan2( R - factor * aDepth, aRadius );

    dAngle = - refAngle / (aNSections + 1);
    depth = R * factor;
    paramSet = true;

    return true;
}


bool DIMPLE::WriteVRML( bool isCapped, bool isCCW, TRANSFORM& aTransform, VRMLMAT& aMaterial,
                        bool reuseMaterial, std::ofstream& aVRMLFile, int aTabDepth )
{
    if( !paramSet )
    {
        ERRBLURB << "invoked without a successful call to SetParams\n";
        return false;
    }

    if( isCapped && isCCW )
    {
        isCapped = false;
        ERRBLURB << "\n[WARNING]: invoked with isCapped and isCCW (makes no sense)\n";
    }

    int fail = 0;

    fail += SetupShape( aMaterial, reuseMaterial, aVRMLFile, aTabDepth );

    if( !writeVertices( isCapped, aTransform, aVRMLFile, aTabDepth + 1 ) )
        ++fail;

    fail += SetupCoordIndex( aVRMLFile, aTabDepth );

    if( writeIndices( isCapped, isCCW, aMaterial, reuseMaterial, aVRMLFile, aTabDepth + 1 ) )
        ++fail;

    fail += CloseCoordIndex( aVRMLFile, aTabDepth );
    fail += CloseShape( aVRMLFile, aTabDepth );

    if( fail )
        return false;

    return true;
}


bool DIMPLE::writeVertices( bool isCapped, TRANSFORM& aTransform,
                            std::ofstream& aVRMLFile, int aTabDepth )
{
    string fmt( aTabDepth * 4, ' ' );
    aVRMLFile << fmt << "coord Coordinate { point [\n";

    double ref = refAngle;
    double sf = abs( sin( ref ) );
    double dz0 = cos( ref );

    int i = 0;  // accumulator for tabs
    QUAT p0(0, 0, 0, 0);

    if(isCapped)
    {
        ++i;
        aTransform.Transform( p0 );

        if( (p0.x < 1e-9) && (p0.x > -1e-9) )
            p0.x = 0;

        if( (p0.y < 1e-9) && (p0.y > -1e-9) )
            p0.y = 0;

        if( (p0.z < 1e-9) && (p0.z > -1e-9) )
            p0.z = 0;

        aVRMLFile << setprecision( 8 ) << " " << p0.y << " " << p0.x << " " << p0.z << ",";
    }

    // write out the first section
    double* px;
    double* py;
    double* pz;
    int k = bump.GetVertices(&px, &py, &pz);
    int l;

    if( k<3 )
    {
        ERRBLURB << "no circular cross-section calculated\n";
        return false;
    }

    for( l = 0; l < k; ++l )
    {
        p0.w = 0;
        p0.x = px[l];
        p0.y = py[l];
        p0.z = pz[l];

        aTransform.Transform( p0 );

        if( (p0.x < 1e-9) && (p0.x > -1e-9) )
            p0.x = 0;

        if( (p0.y < 1e-9) && (p0.y > -1e-9) )
            p0.y = 0;

        if( (p0.z < 1e-9) && (p0.z > -1e-9) )
            p0.z = 0;

        aVRMLFile << setprecision( 8 ) << " " << p0.x << " " << p0.y << " " << p0.z << ",";

        if( !( (++i) % 6 ) )
            aVRMLFile << "\n" << fmt << "   ";
    }

    ref += dAngle;

    int m;
    TRANSFORM t0;
    double sf1;

    for( m = 1; m < sections; ++m )
    {
        sf1 = abs( sin( ref ) );
        t0.SetScale( sf1 / sf );
        t0.SetTranslation( 0, 0, - (( cos( ref ) - dz0)) * depth );

        for( l = 0; l < k; ++l )
        {
            p0.w = 0;
            p0.x = px[l];
            p0.y = py[l];
            p0.z = pz[l];

            t0.Transform( p0 );
            aTransform.Transform( p0 );

            if( (p0.x < 1e-9) && (p0.x > -1e-9) )
                p0.x = 0;

            if( (p0.y < 1e-9) && (p0.y > -1e-9) )
                p0.y = 0;

            if( (p0.z < 1e-9) && (p0.z > -1e-9) )
                p0.z = 0;

            aVRMLFile << setprecision( 8 ) << " " << p0.x << " " << p0.y << " " << p0.z << ",";

            if( !( (++i) % 6 ) )
                aVRMLFile << "\n" << fmt << "   ";
        }

        ref += dAngle;
    }

    // write out the final vertex
    p0.Set( 0, 0, 0, 0 );
    // note: for aesthetic purposes we flatten the sphere a little rather than place a conical vertex
    // t0.SetTranslation( 0, 0, -depth * (1.0 - dz0) );
    t0.SetScale(1);
    t0.Transform(p0);
    aTransform.Transform(p0);

    if( (p0.x < 1e-9) && (p0.x > -1e-9) )
        p0.x = 0;

    if( (p0.y < 1e-9) && (p0.y > -1e-9) )
        p0.y = 0;

    if( (p0.z < 1e-9) && (p0.z > -1e-9) )
        p0.z = 0;

    aVRMLFile << setprecision( 8 ) << " " << p0.x << " " << p0.y << " " << p0.z << " ]\n";
    aVRMLFile << fmt << "}\n";

    return aVRMLFile.good();
}


bool writeTriplet( int v1, int v2, int v3, bool isCCW, std::ofstream& aVRMLFile, char echar )
{
    if( isCCW )
    {
        aVRMLFile << v1 << ", " << v2 << ", " << v3 << ", -1" << echar;
    }
    else
    {
        aVRMLFile << v2 << ", " << v3 << ", " << v1 << ", -1" << echar;
    }

    return aVRMLFile.good();
}


bool DIMPLE::writeIndices( bool isCapped, bool isCCW, VRMLMAT& aMaterial, bool reuseMaterial,
                           std::ofstream& aVRMLFile, int aTabDepth )
{
    int j = 0;  // 0 if not capped, 1 if capped
    int np = sides * sections + 1;
    int acc = 0;    // accumulator for tabs

    string fmt( aTabDepth * 4, ' ' );
    aVRMLFile << fmt;

    if( isCapped )
    {
        ++np;
        j = 1;

        for( int i = 0; i <= sides; ++i )
        {
            writeTriplet( 1, ((i + 1) % sides) + 1, (i %sides) + 1, isCCW, aVRMLFile, ',' );
            if( ++acc >= 6 )
                aVRMLFile << "\n" << fmt;
        }

    }

    int o1;
    int o2 = 0;

    for(int k = 1; k < sections; ++k)
    {
        o2 = j + k*sides;
        o1 = o2 - sides;

        for( int i = 0; i <= sides; ++i )
        {
            writeTriplet( (i %sides) + o1, ((i + 1) % sides) + o1, (i %sides) + o2,
                          isCCW, aVRMLFile, ',' );

            writeTriplet( ((i + 1) % sides) + o1, ((i + 1) % sides) + o2, (i %sides) + o2,
                          isCCW, aVRMLFile, ',' );

            if( ++acc >= 6 )
                aVRMLFile << "\n" << fmt;
        }

    }

    // close the sphere
    o1 = o2 + sides;

    for( int i = 0; i <= sides; ++i )
    {
        writeTriplet( (i %sides) + o2, ((i + 1) % sides) + o2, o1,
                      isCCW, aVRMLFile, ',' );

        if( ++acc >= 6 )
            aVRMLFile << "\n" << fmt;
    }


    return false;
}
