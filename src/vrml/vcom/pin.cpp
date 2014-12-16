/*
 *      file: pin.cpp
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
#include <new>

#include <vdefs.h>
#include <pin.h>
#include <transform.h>
#include <vrmlmat.h>
#include <polygon.h>
#include <circle.h>
#include <rectangle.h>

using namespace std;
using namespace KC3D;

PPARAMS::PPARAMS()
{
    // defaults are in mm for a typical blind pin
    w = 0.6;
    d = 0.6;
    tap = 0.5;
    stw = 0.8;
    std = 0.8;
    dbltap  = false;
    h       = 8.0;
    r       = -1.0; // no bend
    nb      = 5;    // 5 segments to a 90-deg bend
    l       = -1.0; // no horizontal section
    bev     = -1.0; // default to no bevel
    bend    = M_PI / 2.0;
    ns      = 24;

    return;
}


PIN::PIN()
{
    valid = false;
    poly = NULL;
    nr = 0;
    square = true;
}


PIN::PIN( const PIN& aPin )
{
    valid = aPin.valid;
    nr = aPin.nr;
    square = aPin.square;
    poly = NULL;

    if( !valid )
        nr = 0;

    if( nr == 0 )
        return;

    poly = new (nothrow) POLYGON*[nr];

    if( poly == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate memory for polygon pointers\n";
        valid = false;
        nr = 0;
        return;
    }

    int i, j;

    for( i = 0; i < nr; ++i )
    {
        poly[i] = aPin.poly[i]->Clone();

        if( !poly[i] )
        {
            for( j = 0; j < i; ++j )
                delete poly[j];

            delete [] poly;
            poly = NULL;
            ERRBLURB;
            cerr << "could not allocate memory for polygon pointers\n";
            valid = false;
            nr = 0;
            return;
        }
    }
}    // PIN::PIN(const PIN& aPin)


PIN::~PIN()
{
    cleanup();
}


PIN& PIN::operator=( const PIN& aPin )
{
    if( this == &aPin )
        return *this;

    if( valid )
        cleanup();

    valid = aPin.valid;
    nr = aPin.nr;
    square = aPin.square;
    poly = NULL;

    if( !valid )
        nr = 0;

    if( nr == 0 )
        return *this;

    poly = new (nothrow) POLYGON*[nr];

    if( poly == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate memory for polygon pointers\n";
        valid = false;
        nr = 0;
        return *this;
    }

    int i, j;

    for( i = 0; i < nr; ++i )
    {
        poly[i] = aPin.poly[i]->Clone();

        if( !poly[i] )
        {
            for( j = 0; j < i; ++j )
                delete poly[j];

            delete [] poly;
            poly = NULL;
            ERRBLURB;
            cerr << "could not allocate memory for polygon pointers\n";
            valid = false;
            nr = 0;
            return *this;
        }
    }

    return *this;
}    // PIN::operator=(const PIN& aPin)


// Calculate the intermediate polygons
int PIN::Calc( const PPARAMS& aPinParam, TRANSFORM& aTransform )
{
    bool has_h = false; // true if there is a vertical section
    bool has_b = false; // true if there is a bend
    bool has_l = false; // true if there is a horizontal section
    bool has_t = false; // true if there is a taper
    int i;

    if( valid )
        cleanup();

    // validate parameters
    if( aPinParam.h > 0.0 )
    {
        has_h = true;

        if( aPinParam.tap > 0 )
        {
            if( aPinParam.tap > aPinParam.h )
            {
                ERRBLURB;
                cerr << "taper > height\n";
                return -1;
            }

            if( (aPinParam.dbltap) && (aPinParam.r <= 0.0)
                && (aPinParam.tap * 2.0 >= aPinParam.h) )
            {
                ERRBLURB;
                cerr << "double taper >= height\n";
                return -1;
            }

            has_t = true;
        }
    }

    if( aPinParam.l > 0.0 )
    {
        has_l = true;

        if( (aPinParam.dbltap) && (aPinParam.tap > 0) )
        {
            has_t = true;

            if( aPinParam.tap > aPinParam.l )
            {
                ERRBLURB;
                cerr << "taper > length\n";
                return -1;
            }
        }
    }

    if( aPinParam.r > 0 )
    {
        has_b = true;

        if( (aPinParam.nb < 1) || (aPinParam.nb > 90) )
        {
            ERRBLURB;
            cerr << "invalid number of segments in a bend (" << aPinParam.nb;
            cerr << "); valid range is 1 .. 90\n";
            return -1;
        }

        if( aPinParam.r < aPinParam.w )
        {
            ERRBLURB;
            cerr << "bend radius < width\n";
            return -1;
        }

        if( (aPinParam.bend <= 0.0) || (aPinParam.bend > M_PI) )
        {
            ERRBLURB;
            cerr << "invalid number bend angle (" << aPinParam.bend;
            cerr << "); valid range is >0.0 and <=M_PI\n";
            return -1;
        }
    }

    if( aPinParam.bev > 0.0 )
    {
        if( (aPinParam.bev * 2.0 >= aPinParam.w) || (aPinParam.bev * 2.0 >= aPinParam.d) )
        {
            ERRBLURB;
            cerr << "invalid bevel specification (must be < 1/2 of either side)\n";
            return -1;
        }

        if( (aPinParam.tap > 0.0)
            && ( (aPinParam.bev * 2.0 >= aPinParam.w * aPinParam.stw)
            || (aPinParam.bev * 2.0 >= aPinParam.d * aPinParam.std) ) )
        {
            ERRBLURB;
            cerr << "bad bevel specification; bevel will be too large on tapered segment\n";
            return -1;
        }
    }

    if( (!has_b) && (!has_h) && (!has_l) )
    {
        ERRBLURB;
        cerr << "invalid specification; pin has no features\n";
        return -1;
    }

    if( (has_l) && (has_h) && (!has_b) )
    {
        ERRBLURB;
        cerr << "invalid specification; vertical and horizontal bars but no bend\n";
        return -1;
    }

    // calculate the number of polygons needed
    int np = 1;    // this represents the final polygon

    if( has_h )
    {
        ++np;

        if( has_t )
            ++np;

        if( (aPinParam.dbltap) && (aPinParam.r <= 0.0) )
            ++np;
    }

    if( has_b )
        np += aPinParam.nb;

    if( has_l )
    {
        ++np;

        if( (has_t) && (aPinParam.dbltap) )
            ++np;
    }

    poly = new (nothrow) POLYGON*[np];

    if( poly == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate memory for polygon pointers\n";
        return -1;
    }

    if( square )
    {
        int j;

        for( i = 0; i < np; ++i )
        {
            poly[i] = new (nothrow) RECTANGLE( aPinParam.bev );

            if( !poly[i] )
            {
                for( j = 0; j < i; ++j )
                    delete poly[j];

                delete [] poly;
                poly = NULL;
                ERRBLURB;
                cerr << "could not allocate memory for rectangles\n";
                return -1;
            }
        }
    }
    else
    {
        if( (aPinParam.ns < 3) || (aPinParam.ns > 360) )
        {
            ERRBLURB;
            cerr << "invalid number of sides for a polygon (" << aPinParam.ns << ")\n";
            cerr << "\tValid values are 3 .. 360\n";
            return -1;
        }

        int j;

        for( i = 0; i < np; ++i )
        {
            poly[i] = new (nothrow) CIRCLE( aPinParam.ns );

            if( !poly[i] )
            {
                for( j = 0; j < i; ++j )
                    delete poly[j];

                delete [] poly;
                poly = NULL;
                ERRBLURB;
                cerr << "could not allocate memory for circles\n";
                return -1;
            }
        }
    }

    // calculate the polygons
    pin = aPinParam;
    int idx, acc;
    double px, pz;
    idx = 0;
    acc = 0;
    px  = 0.0;
    pz  = 0.0;
    TRANSFORM t0;

    if( has_h )
    {
        if( has_t )
        {
            acc += (*poly[idx++]).Calc( pin.w * pin.stw, pin.d * pin.std, t0 );
            pz  = pin.tap;
            t0.SetTranslation( 0, 0, pz );
        }

        acc += (*poly[idx++]).Calc( pin.w, pin.d, t0 );
        pz  = pin.h;
        t0.SetTranslation( 0, 0, pz );
    }

    if( has_b )
    {
        double ba = pin.bend / pin.nb;  // incremental bend angle
        double tx, tz;                  // x and z positions of the wire centers on the bend
        double ang;

        acc += (*poly[idx++]).Calc( pin.w, pin.d, t0 );

        for( i = 1; i < pin.nb; ++i )
        {
            ang = i * ba;
            tx  = pin.r * ( 1.0 - cos( ang ) );
            tz  = pin.r * sin( ang ) + pz;
            t0.SetRotation( ang, 0, 1, 0 );
            t0.SetTranslation( tx, 0, tz );
            acc += (*poly[idx++]).Calc( pin.w, pin.d, t0 );
        }

        px  = pin.r * ( 1 - cos( pin.bend ) );
        pz  = pz + pin.r * sin( pin.bend );
        t0.SetTranslation( px, 0, pz );
        t0.SetRotation( pin.bend, 0, 1, 0 );
    }

    // Calculate an appropriate transform for the case
    // where only a horizontal section was specified.
    // Using the object in this way is not a recommended
    // means for producing a horizontal pin since the
    // one end will not be tapered under any circumstance
    if( (!has_h) && (!has_b) )
    {
        px  = 0;
        pz  = 0;
        t0.SetTranslation( 0, 0, 0 );
        t0.SetRotation( M_PI / 2.0, 0, 1, 0 );
    }

    if( has_l )
    {
        acc += (*poly[idx++]).Calc( pin.w, pin.d, t0 );

        double cb = M_PI / 2.0 - pin.bend;    // complement to bend

        if( (has_t) && (pin.dbltap) )
        {
            if( pin.r < 0 )
            {
                // special case of a horizontal feature only
                t0.SetTranslation( pin.l - pin.tap, 0, pin.h );
                acc += (*poly[idx++]).Calc( pin.w, pin.d, t0 );
                t0.SetTranslation( pin.l, 0, pin.h );
                acc += (*poly[idx++]).Calc( pin.w * pin.stw, pin.d * pin.std, t0 );
            }
            else
            {
                t0.SetTranslation( px + (pin.l - pin.tap) * cos( cb ), 0,
                        pz + (pin.l - pin.tap) * sin( cb ) );
                acc += (*poly[idx++]).Calc( pin.w, pin.d, t0 );
                t0.SetTranslation( px + pin.l * cos( cb ), 0,
                        pz + pin.l * sin( cb ) );
                acc += (*poly[idx++]).Calc( pin.w * pin.stw, pin.d * pin.std, t0 );
            }
        }
        else
        {
            t0.SetTranslation( px + pin.l * cos( cb ), 0,
                    pz + pin.l * sin( cb ) );
            acc += (*poly[idx++]).Calc( pin.w, pin.d, t0 );
        }
    }
    else
    {
        // add the final polygon to a pin with no horizontal wire
        if( (!has_b) && (has_t) && (pin.dbltap) )
        {
            t0.SetTranslation( 0, 0, pin.h - pin.tap );
            acc += (*poly[idx++]).Calc( pin.w, pin.d, t0 );
            t0.SetTranslation( 0, 0, pin.h );
            acc += (*poly[idx++]).Calc( pin.w * pin.stw, pin.d * pin.std, t0 );
        }
        else
        {
            acc += (*poly[idx++]).Calc( pin.w, pin.d, t0 );
        }
    }

    if( acc < 0 )
    {
        ERRBLURB;
        cerr << "errors encountered while building a square pin\n";
        nr = np;
        cleanup();
        return -1;
    }

    // apply the transform
    for( i = 0; i < np; ++i )
        poly[i]->Xform( aTransform );

    valid = true;
    nr = np;
    return 0;
}    // Calc


// Write out the pin shape information
int PIN::Build( bool aRenderCap0, bool aRenderCap1, TRANSFORM& aFinalTransform,
                VRMLMAT& aMaterial, bool reuseMaterial,
                std::ofstream& aVRMLFile, int aTabDepth )
{
    if( !valid )
    {
        ERRBLURB;
        cerr << "invoked with no prior successful call to Calc()\n";
        return -1;
    }

    if( nr < 2 )
    {
        ERRBLURB;
        cerr << "BUG: invalid number of polygons (min. 2): " << nr << "\n";
        return -1;
    }

    int vl = 0;
    bool reuse = reuseMaterial;

    if( aRenderCap0 )
    {
        vl += poly[0]->Paint( false, aFinalTransform, aMaterial, reuse,
                              aVRMLFile, aTabDepth );
        reuse = true;
    }

    if( aRenderCap1 )
    {
        vl += poly[nr - 1]->Paint( true, aFinalTransform, aMaterial, reuse,
                                   aVRMLFile, aTabDepth);
        reuse = true;
    }

    vl += poly[0]->Stitch( true, *poly[1], aFinalTransform, aMaterial, reuse,
                           aVRMLFile, aTabDepth );

    int i;

    for( i = 1; i < nr - 1; ++i )
    {
        vl += poly[i]->Stitch( true, *poly[i + 1], aFinalTransform, aMaterial, true,
                               aVRMLFile, aTabDepth );
    }

    if( vl )
    {
        ERRBLURB;
        cerr << "problems encountered while rendering part\n";
        return -1;
    }

    return 0;
}


void PIN::SetShape( bool isSquare )
{
    if( valid )
        cleanup();

    PIN::square = isSquare;

    return;
}


void PIN::cleanup( void )
{
    int i;

    if( poly )
    {
        for( i = 0; i < nr; ++i )
            delete poly[i];

        delete [] poly;
        poly = NULL;
    }

    nr = 0;
    valid = false;
}
