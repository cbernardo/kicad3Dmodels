/*
 *      file: circle.cpp
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

#include <vdefs.h>
#include <vcom.h>
#include <polygon.h>
#include <circle.h>
#include <transform.h>
#include <vrmlmat.h>


using namespace std;
using namespace KC3D;

CIRCLE::CIRCLE()
{
    POLYGON::init();
    nv = 16;

    return;
}


CIRCLE::CIRCLE( int nVertices )
{
    POLYGON::init();

    if( (nVertices < 3) || (nVertices > 360) )
    {
        ERRBLURB;
        cerr << "invalid nVertices (" << nVertices << "); range is 3 .. 360\n";
        nv = 16;

        return;
    }

    nv = nVertices;

    return;
}


CIRCLE::CIRCLE( const CIRCLE& aCircle )
{
    valid = aCircle.valid;
    nv = aCircle.nv;

    x = NULL;
    y = NULL;
    z = NULL;

    if( !valid )
        return;

    x = new (nothrow) double[nv];

    if( x == NULL )
    {
        valid = false;
        return;
    }

    y = new (nothrow) double[nv];

    if( y == NULL )
    {
        delete [] x;
        valid = false;
        return;
    }

    z = new (nothrow) double[nv];

    if( z == NULL )
    {
        delete [] x;
        delete [] y;
        valid = false;
        return;
    }

    int i;

    for( i = 0; i < nv; ++i )
    {
        x[i] = aCircle.x[i];
        y[i] = aCircle.y[i];
        z[i] = aCircle.z[i];
    }

    return;
}


CIRCLE& CIRCLE::operator=( const CIRCLE& aCircle )
{
    if( this == &aCircle )
        return *this;

    if( valid )
    {
        delete [] x;
        delete [] y;
        delete [] z;
        nv = 0;
        valid = false;
    }

    valid = aCircle.valid;
    nv = aCircle.nv;

    x = NULL;
    y = NULL;
    z = NULL;

    if( !valid )
        return *this;

    x = new (nothrow) double[nv];

    if( x == NULL )
    {
        valid = false;
        return *this;
    }

    y = new (nothrow) double[nv];

    if( y == NULL )
    {
        delete [] x;
        valid = false;
        return *this;
    }

    z = new (nothrow) double[nv];

    if( z == NULL )
    {
        delete [] x;
        delete [] y;
        valid = false;
        return *this;
    }

    int i;

    for( i = 0; i < nv; ++i )
    {
        x[i] = aCircle.x[i];
        y[i] = aCircle.y[i];
        z[i] = aCircle.z[i];
    }

    return *this;
}


// Clone the object
POLYGON* CIRCLE::Clone( void ) const
{
    return new (nothrow) CIRCLE( *this );
}


CIRCLE::~CIRCLE()
{
    if( x )
        delete [] x;

    if( y )
        delete [] y;

    if( z )
        delete [] z;

    POLYGON::init();
}


#define MIN_RAD (0.000001)
#define MAX_RAD (100.0)


int CIRCLE::Calc( double xDia, double yDia, TRANSFORM& aTransform )
{
    if( valid )
    {
        if( x )
            delete [] x;

        if( y )
            delete [] y;

        if( z )
            delete [] z;

        POLYGON::init();
        valid = false;
    }

    double xrad, yrad;
    xrad = xDia / 2.0;
    yrad = yDia / 2.0;

    if( (xrad < MIN_RAD) || (xrad > MAX_RAD) )
    {
        ERRBLURB;
        cerr << "Invalid X radius (" << xrad << "). Range is ";
        cerr << MIN_RAD << " to " << MAX_RAD << "\n";
        return -1;
    }

    if( (yrad < MIN_RAD) || (yrad > MAX_RAD) )
    {
        ERRBLURB;
        cerr << "Invalid Y radius (" << yrad << "). Range is ";
        cerr << MIN_RAD << " to " << MAX_RAD << "\n";
        return -1;
    }

    x = new (nothrow) double[nv];

    if( x == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate points for vertices\n";
        POLYGON::init();
        return -1;
    }

    y = new (nothrow) double[nv];

    if( y == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate points for vertices\n";
        delete [] x;
        POLYGON::init();
        return -1;
    }

    z = new (nothrow) double[nv];

    if( z == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate points for vertices\n";
        delete [] x;
        delete [] y;
        POLYGON::init();
        return -1;
    }

    // calculate the vertices then apply the transform
    double da   = M_PI / nv * 2.0;
    double ang  = 0.0;
    int i;

    for( i = 0; i < nv; ++i )
    {
        x[i] = xrad * cos( ang );
        y[i] = yrad * sin( ang );
        z[i] = 0.0;
        ang += da;
    }

    // transform the vertices
    aTransform.Transform( x, y, z, nv );

    valid = true;

    return 0;
}    // Calc


int CIRCLE::SetNVertices( int nVert )
{
    if( (nVert < 3) || (nVert > 360) )
    {
        ERRBLURB;
        cerr << "invalid number of vertices (" << nVert << ")\n";
        cerr << "\tValid values are 3 .. 360 only\n";
        return -1;
    }

    if( valid )
    {
        delete [] x;
        delete [] y;
        delete [] z;
        POLYGON::init();
    }

    nv = nVert;

    return 0;
}
