/*
 *      file: hole.cpp
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

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <new>

#include <hole.h>
#include <vdefs.h>
#include <vcom.h>
#include <transform.h>
#include <polygon.h>
#include <circle.h>
#include <rectangle.h>

using namespace std;
using namespace KC3D;

HOLE::HOLE()
{
    valid = false;
    hole    = NULL;
    np  = 16;
    bev = -1.0;
    square = true;
    w1 = 0;
    w2 = 0;
    d1 = 0;
    d2 = 0;
    ow = 0;
    od = 0;
}


HOLE::~HOLE()
{
    if( hole )
        delete hole;
}


int HOLE::Calc( double aPanelWidth, double aPanelDepth, double aHoleWidth, double aHoleDepth,
                TRANSFORM& aTransform, bool isSquare, double aHoleWOff, double aHoleDOff,
                int aNVertex, double aBevel )
{
    if( valid )
    {
        valid = false;
        delete hole;
        hole = NULL;
    }

    // validate the parameters
    if( aPanelWidth <= 0 )
    {
        ERRBLURB;
        cerr << "invalid aPanelWidth: <= 0\n";
        return -1;
    }

    if( (aHoleWidth <= 0) || (w2 >= aPanelWidth) )
    {
        ERRBLURB;
        cerr << "invalid aHoleWidth: <= 0 or >= aPanelWidth\n";
        return -1;
    }

    if( ( (aHoleWidth / 2.0 + aHoleWOff) >= aPanelWidth / 2.0 )
        || ( (aHoleWidth / 2.0 - aHoleWOff) >= aPanelWidth / 2.0 ) )
    {
        ERRBLURB;
        cerr << "invalid parameters: aHoleWidth/2.0 + |aHoleWOff| >= aPanelWidth/2.0\n";
        return -1;
    }

    if( aPanelDepth <= 0 )
    {
        ERRBLURB;
        cerr << "invalid aPanelDepth: <= 0\n";
        return -1;
    }

    if( (aHoleDepth <= 0) || (aHoleDepth >= aPanelDepth) )
    {
        ERRBLURB;
        cerr << "invalid aHoleDepth: <= 0 or >= aPanelDepth\n";
        return -1;
    }

    if( ( (aHoleDepth / 2.0 + aHoleDOff) >= aPanelDepth / 2.0 )
        || ( (aHoleDepth / 2.0 - aHoleDOff) >= aPanelDepth / 2.0 ) )
    {
        ERRBLURB;
        cerr << "invalid parameters: aHoleDepth/2.0 + |aHoleDOff| >= aPanelDepth/2.0\n";
        return -1;
    }

    if( (!isSquare) && ( (aNVertex < 4) || (aNVertex > 360) || (aNVertex % 4) ) )
    {
        ERRBLURB;
        cerr << "invalid aNVertex: must be 4 .. 360 and a multiple of 4\n";
        cerr << "\tUsing default 16\n";
        aNVertex = 16;
    }

    HOLE::square = isSquare;

    if( isSquare )
    {
        if( (aBevel > aHoleDepth / 4.0) || (aBevel > aHoleWidth / 4.0) )
        {
            ERRBLURB;
            cerr << "invalid bevel: must be < min(aHoleDepth, aHoleWidth)/4.0\n";
            return -1;
        }

        HOLE::bev = aBevel;
    }

    // calculate the frame
    double tw, td;
    tw  = aPanelWidth / 2.0;
    td  = aPanelDepth / 2.0;
    v[0][0] = tw;
    v[1][0] = -td;
    v[2][0] = 0.0;

    v[0][1] = tw;
    v[1][1] = td;
    v[2][1] = 0.0;

    v[0][2] = -tw;
    v[1][2] = td;
    v[2][2] = 0.0;

    v[0][3] = -tw;
    v[1][3] = -td;
    v[2][3] = 0.0;

    // calculate the hole
    TRANSFORM t0;
    t0.SetTranslation( aHoleWOff, aHoleDOff, 0.0 );

    if( isSquare )
    {
        hole = new (nothrow) RECTANGLE;

        if( (hole) && (aBevel > 0.0) )
        {
            RECTANGLE* phole = dynamic_cast<RECTANGLE*> (hole);

            if( phole )
                phole->SetBevel( aBevel, 1 );
        }
    }
    else
    {
        hole = new (nothrow) CIRCLE( aNVertex );
    }

    if( !hole )
    {
        ERRBLURB;
        cerr << "could not allocate memory for hole\n";
        return -1;
    }

    hole->Calc( aHoleWidth, aHoleDepth, t0 );

    aTransform.Transform( v[0], v[1], v[2], 4 );
    hole->Xform( aTransform );

    valid = true;

    return 0;
}


int HOLE::Build( bool isTopView, TRANSFORM& aTransform, VRMLMAT& aMaterial, bool reuseMaterial,
               std::ofstream& aVRMLFile, int aTabDepth )
{
    if( !valid )
    {
        ERRBLURB;
        cerr << "invoked with no prior successful call to Calc()\n";
        return -1;
    }

    double* lx, * ly, * lz;
    int np  = hole->GetVertices( &lx, &ly, &lz );
    int np2 = np + 4;

    double* tx, * ty, * tz;

    tx = new (nothrow) double[np2];

    if( !tx )
    {
        ERRBLURB;
        cerr << "cannot allocate memory for vertices\n";
        return -1;
    }

    ty = new (nothrow) double[np2];

    if( !ty )
    {
        ERRBLURB;
        cerr << "cannot allocate memory for vertices\n";
        delete [] tx;
        return -1;
    }

    tz = new (nothrow) double[np2];

    if( !tz )
    {
        ERRBLURB;
        cerr << "cannot allocate memory for vertices\n";
        delete [] tx;
        delete [] ty;
        return -1;
    }

    int i, idx;

    for( i = 0; i < 4; ++i )
    {
        tx[i] = v[0][i];
        ty[i] = v[1][i];
        tz[i] = v[2][i];
    }

    idx = 0;

    for( i = 4; i < np2; ++i, ++idx )
    {
        tx[i] = lx[idx];
        ty[i] = ly[idx];
        tz[i] = lz[idx];
    }

    aTransform.Transform( tx, ty, tz, np2 );

    int acc = 0;

    // set up VRML Shape
    acc += SetupShape( aMaterial, reuseMaterial, aVRMLFile, aTabDepth );

    // enumerate vertices
    acc += WriteCoord( tx, ty, tz, np2, aVRMLFile, aTabDepth + 1 );
    delete [] tx;
    delete [] ty;
    delete [] tz;

    // enumerate facets
    acc += SetupCoordIndex( aVRMLFile, aTabDepth + 1 );

    if( square )
    {
        if( bev > 0.0 )
            acc += writeRFacetsB( isTopView, aVRMLFile, aTabDepth + 2 );
        else
            acc += writeRFacets( isTopView, aVRMLFile, aTabDepth + 2 );
    }
    else
    {
        acc += writeFacets( isTopView, 0, 1, 4 + np * 3 / 4, 3 + np, 4, aVRMLFile, aTabDepth + 2 );
        acc += writeFacets( isTopView, 1, 2, 4, 3 + np / 4, 4 + np / 4, aVRMLFile, aTabDepth + 2 );
        acc += writeFacets( isTopView, 2, 3, 4 + np / 4, 3 + np / 2, 4 + np / 2, aVRMLFile, aTabDepth + 2 );
        acc += writeFacets( isTopView,
                3,
                0,
                4 + np / 2,
                3 + np * 3 / 4,
                4 + np * 3 / 4,
                aVRMLFile,
                aTabDepth + 2,
                true );
    }

    acc += CloseCoordIndex( aVRMLFile, aTabDepth + 1 );
    acc += CloseShape( aVRMLFile, aTabDepth );

    if( acc )
    {
        ERRBLURB;
        cerr << "problems writing data\n";
        return -1;
    }

    return 0;
}


int HOLE::writeFacets( bool top, int v0, int v1, int h0, int h1, int lp,
        std::ofstream& fp, int tabs, bool term )
{
    int i;

    string fmt( tabs * 4, ' ' );

    if( top )
    {
        // render the top
        fp << fmt;

        for( i = h0; i < h1; ++i )
        {
            fp << i << "," << v0 << "," << i + 1 << ",-1,";

            if( !( (i + 1) % 18 ) )
                fp << "\n" << fmt;
        }

        fp << h1 << "," << v0 << "," << lp << ",-1,\n";

        // large triangular facet
        if( term )
            fp << fmt << v0 << "," << v1 << "," << lp << ",-1\n";
        else
            fp << fmt << v0 << "," << v1 << "," << lp << ",-1,\n";
    }
    else
    {
        // render the bottom
        fp << fmt;

        for( i = h0; i < h1; ++i )
        {
            fp << v0 << "," << i << "," << i + 1 << ",-1,";

            if( !( (i + 1) % 18 ) )
                fp << "\n" << fmt;
        }

        fp << v0 << "," << h1 << "," << lp << ",-1,\n";

        // large triangular facet
        if( term )
            fp << fmt << v1 << "," << v0 << "," << lp << ",-1\n";
        else
            fp << fmt << v1 << "," << v0 << "," << lp << ",-1,\n";
    }

    if( !fp.good() )
        return -1;

    return 0;
}


int HOLE::writeRFacets( bool top, std::ofstream& fp, int tabs )
{
    string fmt( tabs * 4, ' ' );

    if( top )
    {
        fp << fmt;
        fp << "0,1,5,4,-1,1,2,6,5,-1,2,3,7,6,-1,3,0,4,7,-1,\n";
    }
    else
    {
        fp << fmt;
        fp << "1,0,4,5,-1,2,1,5,6,-1,3,2,6,7,-1,0,3,7,4,-1\n";
    }

    if( !fp.good() )
        return -1;

    return 0;
}


int HOLE::writeRFacetsB( bool top, std::ofstream& fp, int tabs )
{
    string fmt( tabs * 4, ' ' );

    if( top )
    {
        fp << fmt;
        fp << "0,5,4,-1,0,1,6,5,-1,1,7,6,-1,1,2,8,7,-1,";
        fp << "2,9,8,-1,2,3,10,9,-1, 3,11,10,-1,3,0,4,11,-1\n";
    }
    else
    {
        fp << fmt;
        fp << "0,4,5,-1,1,0,5,6,-1,1,6,7,-1,2,1,7,8,-1,";
        fp << "2,8,9,-1,3,2,9,10,-1, 3,10,11,-1,0,3,11,4,-1\n";
    }

    if( !fp.good() )
        return -1;

    return 0;
}
