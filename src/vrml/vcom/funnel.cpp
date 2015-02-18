/*
 *      file: funnel.cpp
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
 *      This class represents a rectangular or circular funnel which can
 *      be used to create recessed pins.
 *
 */

#include <iostream>
#include <fstream>
#include <new>

#include <vdefs.h>
#include <vcom.h>
#include <funnel.h>
#include <transform.h>
#include <circle.h>
#include <rectangle.h>
#include <vrmlmat.h>

using namespace std;
using namespace KC3D;

FUNNEL::FUNNEL()
{
    valid  = false;
    square = true;
    npoly  = 0;
    poly   = NULL;
    bev    = 0;
}


FUNNEL::FUNNEL( const FUNNEL& p )
{
    valid  = p.valid;
    square = p.square;
    npoly  = p.npoly;
    bev    = p.bev;
    poly   = NULL;

    if( !valid )
        npoly = 0;

    if( npoly == 0 )
        return;

    poly = new (nothrow) POLYGON*[npoly];

    if( poly == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate memory for polygon pointers\n";
        valid = false;
        npoly = 0;
        return;
    }

    int i, j;

    for( i = 0; i < npoly; ++i )
    {
        poly[i] = p.poly[i]->Clone();

        if( !poly[i] )
        {
            for( j = 0; j < i; ++j )
                delete poly[j];

            delete [] poly;
            poly = NULL;
            ERRBLURB;
            cerr << "could not allocate memory for polygon pointers\n";
            valid = false;
            npoly = 0;
            return;
        }
    }
}


FUNNEL::~FUNNEL()
{
    cleanup();
}


FUNNEL& FUNNEL::operator=( const FUNNEL& p )
{
    if( this == &p )
        return *this;

    if( valid )
        cleanup();

    valid = p.valid;
    square  = p.square;
    npoly   = p.npoly;
    poly = NULL;

    if( !valid )
        npoly = 0;

    if( npoly == 0 )
        return *this;

    poly = new (nothrow) POLYGON*[npoly];

    if( poly == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate memory for polygon pointers\n";
        valid = false;
        npoly = 0;
        return *this;
    }

    int i, j;

    for( i = 0; i < npoly; ++i )
    {
        poly[i] = p.poly[i]->Clone();

        if( !poly[i] )
        {
            for( j = 0; j < i; ++j )
                delete poly[j];

            delete [] poly;
            poly = NULL;
            ERRBLURB;
            cerr << "could not allocate memory for polygon pointers\n";
            valid = false;
            npoly = 0;
            return *this;
        }
    }

    return *this;
}


int FUNNEL::Calc( double aFluteXWidth, double aFluteYDepth, double aStemXWidth,
                  double aStemYDepth, double aFluteLength, double aStemLength,
                  double aStemLength2, TRANSFORM& aTransform, int aNumberSides )
{
    if( valid )
        cleanup();

    int np;
    bool has_h2 = false;

    if( aStemLength > 0.0 )
    {
        has_h2 = true;
        np = 4;
    }
    else
    {
        np = 3;
    }

    // validate parameters
    if( (square) && (bev > 0.0) )
    {
        if( bev * 2.0 >= aStemYDepth )
        {
            ERRBLURB;
            cerr << "invalid for aStemYDepth (<= bevel/2)\n";
            return -1;
        }

        if( bev * 2.0 >= aStemXWidth )
        {
            ERRBLURB;
            cerr << "invalid for aStemXWidth (<= bevel/2)\n";
            return -1;
        }
    }

    if( aFluteXWidth <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid value for aFluteXWidth (<= 0)\n";
        return -1;
    }

    if( aStemXWidth <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid value for aStemXWidth (<= 0)\n";
        return -1;
    }

    if( aFluteXWidth <= aStemXWidth )
    {
        ERRBLURB;
        cerr << "invalid value for aFluteXWidth (<= aStemXWidth)\n";
        return -1;
    }

    if( aFluteYDepth <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid value for aFluteYDepth (<= 0)\n";
        return -1;
    }

    if( aStemYDepth <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid value for aStemYDepth (<= 0)\n";
        return -1;
    }

    if( aFluteYDepth <= aStemYDepth )
    {
        ERRBLURB;
        cerr << "invalid value for aFluteYDepth (<= aStemYDepth)\n";
        return -1;
    }

    if( aFluteLength < 0.0 )
    {
        ERRBLURB;
        cerr << "invalid value for aFluteLength (< 0)\n";
        return -1;
    }

    if( aStemLength2 <= 0.0 )
    {
        ERRBLURB;
        cerr << "invalid value for aStemLength2 (<= 0)\n";
        return -1;
    }

    poly = new (nothrow) POLYGON*[np];

    if( !poly )
    {
        ERRBLURB;
        cerr << "could not allocate memory for polygon pointers\n";
        return -1;
    }

    int i, j;

    if( square )
    {
        for( i = 0; i < np; ++i )
        {
            poly[i] = new (nothrow) RECTANGLE( -1.0 );

            if( !poly[i] )
            {
                ERRBLURB;
                cerr << "could not allocate memory for polygons\n";

                for( j = 0; j < i; ++j )
                    delete poly[j];

                delete [] poly;
                poly = NULL;
                return -1;
            }

            static_cast <RECTANGLE*> (poly[i])->SetBevel( bev );
        }
    }
    else
    {
        int cs = aNumberSides;

        if( (cs < 3) || (cs > 360) || (cs % 4) )
        {
            ERRBLURB;
            cerr << "invalid number of sides (" << aNumberSides <<
            "); valid range is 4 .. 360 in multiples of 4\n";
            cerr << "\tUsing default 16\n";
            cs = 16;
        }

        for( i = 0; i < np; ++i )
        {
            poly[i] = new (nothrow) CIRCLE( cs );

            if( !poly[i] )
            {
                ERRBLURB;
                cerr << "could not allocate memory for polygons\n";

                for( j = 0; j < i; ++j )
                    delete poly[j];

                delete [] poly;
                poly = NULL;
                return -1;
            }
        }
    }

    int acc = 0;
    int idx = 0;
    TRANSFORM t0;
    acc += poly[idx++]->Calc( aFluteXWidth, aFluteYDepth, t0 );
    t0.SetTranslation( 0, 0, -aFluteLength );
    acc += poly[idx++]->Calc( aStemXWidth, aStemYDepth, t0 );

    if( has_h2 )
    {
        t0.SetTranslation( 0, 0, -(aFluteLength + aStemLength) );
        acc += poly[idx++]->Calc( aStemXWidth, aStemYDepth, t0 );
        t0.SetTranslation( 0, 0, -(aFluteLength + aStemLength + aStemLength2) );
    }
    else
    {
        t0.SetTranslation( 0, 0, -(aFluteLength + aStemLength2) );
    }

    acc += poly[idx++]->Calc( aStemXWidth, aStemYDepth, t0 );

    npoly = np;
    valid = true;

    if( acc )
    {
        ERRBLURB;
        cerr << "problems encountered while creatign funnel\n";
        cleanup();
        return -1;
    }

    // apply the transform
    for( i = 0; i < np; ++i )
        poly[i]->Xform( aTransform );

    return 0;
}


int FUNNEL::Build( bool aRenderCap, TRANSFORM& aTransform, VRMLMAT& aFluteMat,
                   bool reuseFluteMat, VRMLMAT& aStemMat, bool reuseStemMat,
                   std::ofstream& aVRMLFile, int aTabDepth )
{
    if( !valid )
    {
        ERRBLURB;
        cerr << "invoked with no prior successful call to Calc()\n";
        return -1;
    }

    if( npoly < 2 )
    {
        ERRBLURB;
        cerr << "BUG: invalid number of polygons (min. 2): " << npoly << "\n";
        return -1;
    }

    bool has_h2 = false;

    if( npoly == 4 )
        has_h2 = true;

    int acc = 0;
    int idx = 0;
    bool reuse = reuseFluteMat;
    acc += poly[idx]->Stitch( true, *poly[idx + 1], aTransform, aFluteMat, reuse,
                              aVRMLFile, aTabDepth );
    ++idx;

    if( has_h2 )
    {
        acc += poly[idx]->Stitch( true, *poly[idx + 1], aTransform, aFluteMat, true,
                                  aVRMLFile, aTabDepth );
        ++idx;
    }

    // stem
    reuse = reuseStemMat;

    if( !aFluteMat.GetName().compare( aStemMat.GetName() ) )
    {
        reuse = true;
    }

    acc += poly[idx]->Stitch( true, *poly[idx + 1], aTransform, aStemMat, reuse,
                              aVRMLFile, aTabDepth );
    ++idx;

    if( aRenderCap )
    {
        acc += poly[idx]->Paint( true, aTransform, aStemMat, true, aVRMLFile, aTabDepth );
    }

    if( acc )
    {
        ERRBLURB;
        cerr << "problems encountered creating funnel\n";
        return -1;
    }

    return 0;
}    // Build()


void FUNNEL::cleanup( void )
{
    int i;

    if( valid )
    {
        for( i = 0; i < npoly; ++i )
            delete poly[i];

        delete [] poly;
        poly = NULL;
    }

    valid = false;
    npoly = 0;

    return;
}


void FUNNEL::SetShape( bool isSquare, double aBevel )
{
    if( valid )
        cleanup();

    FUNNEL::square = isSquare;
    FUNNEL::bev = aBevel;

    return;
}
