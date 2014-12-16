/*
 *      file: polygon.cpp
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
#include <iomanip>
#include <new>
#include <algorithm>

#include <vdefs.h>
#include <vcom.h>
#include <polygon.h>
#include <transform.h>
#include <vrmlmat.h>


using namespace std;
using namespace KC3D;

POLYGON::POLYGON()
{
    nv = 0;
    init();

    return;
}


POLYGON::~POLYGON()
{
    if( x )
        delete [] x;

    if( y )
        delete [] y;

    if( z )
        delete [] z;

    return;
}


bool POLYGON::IsValid( void ) const
{
    return valid;
}


void POLYGON::init( void )
{
    x = y = z = NULL;
    valid = false;

    return;
}


int POLYGON::Paint( bool isCCW, TRANSFORM& aTransform, VRMLMAT& aMaterial, bool reuseMaterial,
                   std::ofstream& aVRMLFile, int aTabDepth )
{
    int i;

    if( !valid )
    {
        ERRBLURB;
        cerr << "invoked without prior invocation of Calc()\n";
        return -1;
    }

    if( nv < 3 )
    {
        ERRBLURB;
        cerr << "invalid number of vertices (" << nv << "); range is 3 .. 360\n";
        return -1;
    }

    if( aTabDepth < 0 )
        aTabDepth = 0;

    if( aTabDepth > 4 )
        aTabDepth = 4;

    double* lx, * ly, * lz;

    // allocate memory
    lx = new (nothrow) double[nv];

    if( lx == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate points for intermediate calculations\n";
        return -1;
    }

    ly = new (nothrow) double[nv];

    if( ly == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate points for intermediate calculations\n";
        delete [] lx;
        return -1;
    }

    lz = new (nothrow) double[nv];

    if( lz == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate points for intermediate calculations\n";
        delete [] lx;
        delete [] ly;
        return -1;
    }

    for( i = 0; i < nv; ++i )
    {
        lx[i] = x[i];
        ly[i] = y[i];
        lz[i] = z[i];
    }

    // perform transforms
    aTransform.Transform( lx, ly, lz, nv );

    // set up VRML Shape
    SetupShape( aMaterial, reuseMaterial, aVRMLFile, aTabDepth );
    // enumerate vertices
    WriteCoord( lx, ly, lz, nv, aVRMLFile, aTabDepth + 1 );
    // enumerate facets
    SetupCoordIndex( aVRMLFile, aTabDepth + 1 );
    string fmt( (aTabDepth + 1) * 4, ' ' );
    aVRMLFile << fmt << "    ";

    if( isCCW )
    {
        for( i = 0; i < nv - 1; ++i )
        {
            aVRMLFile << i << ",";

            if( !( (i + 1) % 18 ) )
                aVRMLFile << "\n" << fmt << "    ";
        }

        aVRMLFile << i << ",0,-1\n";
    }
    else
    {
        for( i = nv - 1; i > 0; --i )
        {
            aVRMLFile << i << ",";

            if( !( (i + 1) % 18 ) )
                aVRMLFile << "\n" << fmt << "    ";
        }

        aVRMLFile << "0," << nv - 1 << ",-1\n";
    }

    CloseCoordIndex( aVRMLFile, aTabDepth + 1 );

    delete [] lx;
    delete [] ly;
    delete [] lz;

    return CloseShape( aVRMLFile, aTabDepth );
}


int POLYGON::Stitch( bool isCCW, POLYGON& aPolygon, TRANSFORM& aTransform, VRMLMAT& aMaterial,
                     bool reuseMaterial, std::ofstream& aVRMLFile, int aTabDepth )
{
    int i, j, k;

    if( !valid )
    {
        ERRBLURB;
        cerr << "invoked without prior invocation of Calc()\n";
        return -1;
    }

    if( nv < 3 )
    {
        ERRBLURB;
        cerr << "invalid number of vertices (" << nv << "); range is 3 .. 360\n";
        return -1;
    }

    double* r2x = NULL;
    double* r2y = NULL;
    double* r2z = NULL;
    int rv = aPolygon.GetVertices( &r2x, &r2y, &r2z );

    if( rv != nv )
    {
        ERRBLURB;
        cerr << "points in second ring (" << rv << ") do not match points in this ring (" << nv <<
        ")\n";
        return -1;
    }

    if( aTabDepth < 0 )
        aTabDepth = 0;

    if( aTabDepth > 4 )
        aTabDepth = 4;

    double* lx, * ly, * lz;
    lx = ly = lz = NULL;

    // allocate memory
    lx = new double[nv * 2];

    if( lx == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate points for intermediate calculations\n";
        return -1;
    }

    ly = new double[nv * 2];

    if( ly == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate points for intermediate calculations\n";
        delete [] lx;
        return -1;
    }

    lz = new double[nv * 2];

    if( lz == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate points for intermediate calculations\n";
        delete [] lx;
        delete [] ly;
        return -1;
    }

    for( i = 0; i < nv; ++i )
    {
        lx[i] = x[i];
        ly[i] = y[i];
        lz[i] = z[i];
        lx[i + nv]  = r2x[i];
        ly[i + nv]  = r2y[i];
        lz[i + nv]  = r2z[i];
    }

    // perform transforms
    aTransform.Transform( lx, ly, lz, nv * 2 );

    // set up VRML Shape
    SetupShape( aMaterial, reuseMaterial, aVRMLFile, aTabDepth );
    // enumerate vertices
    WriteCoord( lx, ly, lz, nv * 2, aVRMLFile, aTabDepth + 1 );
    // enumerate facets
    SetupCoordIndex( aVRMLFile, aTabDepth + 1 );
    string fmt( (aTabDepth + 1) * 4, ' ' );
    aVRMLFile << fmt << "   ";

    if( isCCW )
        k = 1;
    else
        k = -1;

    for( i = 0; i < nv - 1; ++i )
    {
        j = i + k;

        if( j >= nv )
            j -= nv;

        if( j < 0 )
            j += nv;

        aVRMLFile << " " << i << "," << j << "," << j + nv << "," << i + nv << ",-1,";

        if( !( (i + 1) % 4 ) )
            aVRMLFile << "\n" << fmt << "   ";
    }

    j = i + k;

    if( j >= nv )
        j -= nv;

    if( j < 0 )
        j += nv;

    aVRMLFile << " " << i << "," << j << "," << j + nv << "," << i + nv << ",-1\n";

    CloseCoordIndex( aVRMLFile, aTabDepth + 1 );

    delete [] lx;
    delete [] ly;
    delete [] lz;

    return CloseShape( aVRMLFile, aTabDepth );
}


// Transform all points in the polygon
int POLYGON::Xform( TRANSFORM& aTransform )
{
    if( !valid )
    {
        ERRBLURB;
        cerr << "invoked without prior invocation of Calc()\n";
        return -1;
    }

    aTransform.Transform( x, y, z, nv );
    return 0;
}


// Return value: number of points. Handles will point to arrays of doubles
int POLYGON::GetVertices( double** px, double** py, double** pz ) const
{
    *px = x;
    *py = y;
    *pz = z;

    return nv;
}


bool POLYGON::Reverse( void )
{
    if( nv == 0 ) return false;

    int mid = nv / 2;
    int max = nv -1;

    int i;
    for(i = 0; i < mid;)
    {
        swap(x[i], x[max]);
        swap(y[i], y[max]);
        swap(z[i++], z[max--]);
    }

    return true;
}


int POLYGON::Extrude( bool aRenderCap0,
                      bool aRenderCap1,
                      bool isCCW,
                      QUAT& aStartPoint,
                      TRANSFORM& aTransform,
                      TRANSFORM& aFinalTransform,
                      VRMLMAT& aMaterial,
                      bool reuseMaterial,
                      std::ofstream& aVRMLFile,
                      int aTabDepth )
{
    SIMPLEPOLY pp( *this );

    if( !pp.IsValid() )
    {
        ERRBLURB;
        cerr << "could not duplicate polygon\n";
        return -1;
    }

    TRANSFORM tx;
    tx.SetTranslation( -aStartPoint );
    // move the shape back to the global origin
    int acc = pp.Xform( tx );
    // transform the shape
    acc += pp.Xform( aTransform );
    // shift the shape back
    tx.SetTranslation( aStartPoint );
    acc += pp.Xform( tx );

    if( isCCW )
    {
        acc += this->Stitch( true, pp, aFinalTransform, aMaterial, reuseMaterial,
                             aVRMLFile, aTabDepth );

        if( aRenderCap0 )
            acc += this->Paint( false, aFinalTransform, aMaterial, true,
                                aVRMLFile, aTabDepth );

        if( aRenderCap1 )
            acc += pp.Paint( true, aFinalTransform, aMaterial, true, aVRMLFile, aTabDepth );

    }
    else
    {
        acc += this->Stitch( false, pp, aFinalTransform, aMaterial, reuseMaterial,
                             aVRMLFile, aTabDepth );

        if( aRenderCap0 )
            acc += this->Paint( true, aFinalTransform, aMaterial, true,
                                aVRMLFile, aTabDepth );

        if( aRenderCap1 )
            acc += pp.Paint( false, aFinalTransform, aMaterial, true, aVRMLFile, aTabDepth );

    }

    if( acc )
    {
        ERRBLURB;
        cerr << "problems creating the extrusion\n";
        return -1;
    }

    return 0;
}


POLYGON* FAKEPOLY::Clone( void ) const
{
    ERRBLURB;
    cerr << "\nBUG: FAKEPOLY cannot be cloned, it must not be treated as a derived POLYGON\n";

    return NULL;
}


int FAKEPOLY::Calc( double xl, double yl, TRANSFORM& t )
{
    ERRBLURB;
    cerr << "BUG: FakePoly does not support Calc(), it must not be treated as a derived Polygon\n";

    return -1;
}


FAKEPOLY::~FAKEPOLY()
{
    // Do not allow the Polygon destructor to free memory
    POLYGON::x  = NULL;
    POLYGON::y  = NULL;
    POLYGON::z  = NULL;
    POLYGON::nv = 0;
    POLYGON::valid = false;

    return;
}


void FAKEPOLY::SetParams( double* x, double* y, double* z, int np, bool valid )
{
    POLYGON::x  = x;
    POLYGON::y  = y;
    POLYGON::z  = z;
    POLYGON::nv = np;
    POLYGON::valid = valid;

    return;
}


SIMPLEPOLY::SIMPLEPOLY()
{
    memReserved = 0;
}


SIMPLEPOLY::~SIMPLEPOLY()
{
}


SIMPLEPOLY::SIMPLEPOLY( const POLYGON& aPolygon )
{
    if( SetValues( aPolygon ) )
        POLYGON::init();
}


SIMPLEPOLY::SIMPLEPOLY( const SIMPLEPOLY& aSimplePoly )
{
    if( SetValues( aSimplePoly ) )
        POLYGON::init();
}


// inherited methods
POLYGON* SIMPLEPOLY::Clone( void ) const
{
    ERRBLURB;
    cerr << "BUG: SimplePoly cannot be cloned, it must not be treated as a derived Polygon\n";
    return NULL;
}


int SIMPLEPOLY::Calc( double xFactor, double yFactor, TRANSFORM& aTransform )
{
    ERRBLURB;
    cerr <<
    "BUG: SimplePoly does not support Calc(), it must not be treated as a derived Polygon\n";
    return -1;
}


// extended methods
int SIMPLEPOLY::SetValues( const POLYGON& aPolygon )
{
    if( x )
        delete [] x;

    if( y )
        delete [] y;

    if( z )
        delete [] z;

    x = y = z = NULL;
    nv = 0;
    valid = false;

    int np;
    double* px, * py, * pz;
    px  = py = pz = NULL;
    np  = aPolygon.GetVertices( &px, &py, &pz );

    if( np < 3 )
        return 0;         // polygons must have 3 or more vertices

    if( (!px) || (!py) || (!pz) )
    {
        ERRBLURB;
        cerr << "invalid pointer to coordinate\n";
        return -1;
    }

    x = new (nothrow) double[np];

    if( x == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate memory for vertices\n";
        return -1;
    }

    y = new (nothrow) double[np];

    if( y == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate memory for vertices\n";
        delete [] x;
        return -1;
    }

    z = new (nothrow) double[np];

    if( z == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate memory for vertices\n";
        delete [] x;
        delete [] y;
        return -1;
    }

    int i;

    for( i = 0; i < np; ++i )
    {
        x[i] = px[i];
        y[i] = py[i];
        z[i] = pz[i];
    }

    valid = true;
    nv = np;
    return 0;
}


int SIMPLEPOLY::AddVertex( double X, double Y, double Z )
{
    if( nv >= memReserved )
    {
        int np = nv + 10;

        double* lx = new (nothrow) double[np];

        if( !lx )
            return -1;

        double* ly = new (nothrow) double[np];

        if( !ly )
            return -1;

        double* lz = new (nothrow) double[np];

        if( !lz )
            return -1;

        if( nv )
        {
            // copy data
            int i;

            for( i = 0; i < nv; ++i )
            {
                lx[i] = x[i];
                ly[i] = y[i];
                lz[i] = z[i];
            }

            delete [] x;
            delete [] y;
            delete [] z;
        }

        memReserved = np;
        x = lx;
        y = ly;
        z = lz;
    }

    x[nv] = X;
    y[nv] = Y;
    z[nv++] = Z;

    if( nv == 3 ) valid = true;

    return 0;
}
