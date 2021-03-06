/*
 *      file: hdrbase.cpp
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
#include <transform.h>
#include <vrmlmat.h>
#include <hdrbase.h>
#include <hole.h>

using namespace std;
using namespace KC3D;

// 0.3mm is a paper thin header; presumably we will have no such thing
#define MIN_HDR_HEIGHT (0.3)

// minimum clearance between hole and edge
#define MIN_CLR (0.1)


HDRBASE::HDRBASE()
{
    x = y = z = NULL;
    nv = 0;
    valid = false;

    setDefaults();
}


HDRBASE::HDRBASE( double bevel )
{
    x = y = z = NULL;
    nv = 0;
    valid = false;

    setDefaults();
    bev = bevel;
}


HDRBASE::HDRBASE( const HDRBASE& p )
{
    // put the base class into a known state
    POLYGON::x  = NULL;
    POLYGON::y  = NULL;
    POLYGON::z  = NULL;
    POLYGON::nv = 0;

    valid = p.valid;
    ns  = p.ns;

    nv  = p.nv;
    x   = NULL;
    y   = NULL;
    z   = NULL;

    bev = p.bev;
    height  = p.height;
    xpins   = p.xpins;
    ypins   = p.ypins;
    hd0 = p.hd0;
    hd1 = p.hd1;
    sh  = p.sh;
    xpitch  = p.xpitch;
    ypitch  = p.ypitch;
    squarebot = p.squarebot;
    squaretop = p.squaretop;
    pbev = p.pbev;
    fbev = p.fbev;
    hdy = p.hdy;
    hassh = p.hassh;
    male = p.male;

    if( !valid )
        return;

    // allocate nv*2 vertices since we have 2 polygons
    x = new (nothrow) double[nv * 2];

    if( x == NULL )
    {
        valid = false;
        return;
    }

    y = new (nothrow) double[nv * 2];

    if( y == NULL )
    {
        delete [] x;
        valid = false;
        return;
    }

    z = new (nothrow) double[nv * 2];

    if( z == NULL )
    {
        delete [] x;
        delete [] y;
        valid = false;
        return;
    }

    int i;

    for( i = 0; i < nv * 2; ++i )
    {
        x[i] = p.x[i];
        y[i] = p.y[i];
        z[i] = p.z[i];
        x[i + nv] = p.x[i + nv];
        y[i + nv] = p.y[i + nv];
        z[i + nv] = p.z[i + nv];
    }

    for( i = 0; i < 4; ++i )
    {
        sv[0][i] = p.sv[0][i];
        sv[1][i] = p.sv[1][i];
        sv[2][i] = p.sv[2][i];
        sv[0][i + 4] = p.sv[0][i + 4];
        sv[1][i + 4] = p.sv[1][i + 4];
        sv[2][i + 4] = p.sv[2][i + 4];
    }
}


HDRBASE::~HDRBASE()
{
    if( x )
        delete [] x;

    if( y )
        delete [] y;

    if( z )
        delete [] z;

    x = y = z = NULL;
    nv = 0;
    // set base class parameters to avoid double delete
    POLYGON::x  = NULL;
    POLYGON::y  = NULL;
    POLYGON::z  = NULL;
    POLYGON::nv = 0;
    POLYGON::valid = false;
}


HDRBASE& HDRBASE::operator=( const HDRBASE& p )
{
    if( this == &p )
        return *this;

    if( valid )
    {
        if( x )
            delete [] x;

        if( y )
            delete [] y;

        if( z )
            delete [] z;

        x = y = z = 0;
        nv = 0;
        valid = false;
    }

    // put the base class into a known state
    POLYGON::x  = NULL;
    POLYGON::y  = NULL;
    POLYGON::z  = NULL;
    POLYGON::nv = 0;
    valid = p.valid;

    nv  = p.nv;
    x   = NULL;
    y   = NULL;
    z   = NULL;

    bev = p.bev;
    height  = p.height;
    xpins   = p.xpins;
    ypins   = p.ypins;
    hd0 = p.hd0;
    hd1 = p.hd1;
    sh  = p.sh;
    xpitch  = p.xpitch;
    ypitch  = p.ypitch;
    squarebot = p.squarebot;
    squaretop = p.squaretop;
    pbev = p.pbev;
    fbev = p.fbev;
    hdy = p.hdy;
    hassh = p.hassh;
    male = p.male;

    if( !valid )
        return *this;

    x = new (nothrow) double[nv * 2];

    if( x == NULL )
    {
        valid = false;
        return *this;
    }

    y = new (nothrow) double[nv * 2];

    if( y == NULL )
    {
        delete [] x;
        valid = false;
        return *this;
    }

    z = new (nothrow) double[nv * 2];

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
        x[i] = p.x[i];
        y[i] = p.y[i];
        z[i] = p.z[i];
        x[i + nv] = p.x[i + nv];
        y[i + nv] = p.y[i + nv];
        z[i + nv] = p.z[i + nv];
    }

    for( i = 0; i < 4; ++i )
    {
        sv[0][i] = p.sv[0][i];
        sv[1][i] = p.sv[1][i];
        sv[2][i] = p.sv[2][i];
        sv[0][i + 4] = p.sv[0][i + 4];
        sv[1][i + 4] = p.sv[1][i + 4];
        sv[2][i + 4] = p.sv[2][i + 4];
    }

    return *this;
}


POLYGON* HDRBASE::Clone( void ) const
{
    return new (nothrow) HDRBASE( *this );
}


void HDRBASE::setDefaults( void )
{
    // default header settings
    bev = 0.4;
    height  = 2.72;
    xpins   = 3;
    ypins   = 1;
    squarebot = true;
    squaretop = true;
    male = true;
    hd0 = 0.64;
    hdy = 0.64;
    hd1 = 1.6;
    sh  = 0.72;
    hassh = true;
    xpitch  = 2.54;
    ypitch  = 2.54;
    ns = 16;
    male = true;
    pbev = -1.0;
    fbev = -1.0;
}


int HDRBASE::SetParams( double aXPitch, double aYPitch, double aBevel, double aHeight,
                        double aShoulderHeight, bool aRenderShoulder,
                        double aBotHoleXDim, double aBotHoleYDim, double aTopHoleXDim,
                        bool isSquareBot, bool isSquareTop, bool isMale,
                        double aBotHoleBev, double aTopHoleBev,
                        int aNCols, int aNRows, int aNVertex )
{
    if( valid )
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
    }

    POLYGON::x  = NULL;
    POLYGON::y  = NULL;
    POLYGON::z  = NULL;
    POLYGON::nv = 0;

    // note: bevel parameter can only be checked on invoking Calc
    HDRBASE::bev = aBevel;

    if( aHeight < MIN_HDR_HEIGHT )
    {
        ERRBLURB;
        cerr << "invalid header height (< MIN_HDR_HEIGHT)\n";
        return -1;
    }

    HDRBASE::height = aHeight;

    if( isSquareBot && ( (aBotHoleBev > aBotHoleXDim / 4.0)
        || (aBotHoleBev > aBotHoleYDim / 4.0) ) )
    {
        ERRBLURB;
        cerr << "aBotHoleBev must be < min(aBotHoleXDim, aBotHoleYDim)/4\n";
        return -1;
    }

    HDRBASE::pbev = aBotHoleBev;

    if( !isMale )
    {
        if( aTopHoleBev > aTopHoleXDim / 3.0 )
        {
            ERRBLURB;
            cerr << "aTopHoleBev must be <= aTopHoleXDim/3\n";
            return -1;
        }

        HDRBASE::fbev = aTopHoleBev;
    }

    if( aNCols < 1 )
    {
        ERRBLURB;
        cerr << "invalid number of columns (< 1)\n";
        return -1;
    }

    HDRBASE::xpins = aNCols;

    if( aNRows < 1 )
    {
        ERRBLURB;
        cerr << "invalid number of rows (< 1)\n";
        return -1;
    }

    HDRBASE::ypins = aNRows;

    HDRBASE::squarebot  = isSquareBot;
    HDRBASE::squaretop  = isSquareTop;
    HDRBASE::male = isMale;

    // note: hole dimensions can only be checked for validity
    // within Calc()
    if( aBotHoleXDim <= 0 )
    {
        ERRBLURB;
        cerr << "invalid hole dimension for bottom (<= 0)\n";
        return -1;
    }

    HDRBASE::hd0 = aBotHoleXDim;

    if( isSquareBot )
    {
        if( aBotHoleYDim < aBotHoleXDim / 10.0 )
        {
            ERRBLURB;
            cerr << "aBotHoleYDim must be >= aBotHoleXDim/10\n";
            return -1;
        }

        HDRBASE::hdy = aBotHoleYDim;
    }
    else
    {
        // round pins may only be circular
        HDRBASE::hdy = aBotHoleXDim;
    }

    // force male pins to have equal dimensions on top and bottom
    if( isMale )
        aTopHoleXDim = aBotHoleXDim;

    if( aTopHoleXDim <= 0 )
    {
        ERRBLURB;
        cerr << "invalid hole dimension for top (<= 0)\n";
        return -1;
    }

    HDRBASE::hd1 = aTopHoleXDim;

    if( (aShoulderHeight > 0.0) && ( aShoulderHeight > (aHeight - MIN_HDR_HEIGHT) ) )
    {
        ERRBLURB;
        cerr << "invalid shoulder height (> aHeight - MIN_HDR_HEIGHT)\n";
        return -1;
    }

    HDRBASE::sh = aShoulderHeight;

    if( aShoulderHeight > 1e-9 )
        HDRBASE::hassh = aRenderShoulder;
    else
        HDRBASE::hassh = false;

    if( (!isSquareBot) || (!isSquareTop) )
    {
        if( (aNVertex < 3) || (aNVertex > 360) )
        {
            ERRBLURB;
            cerr << "invalid number of sides (ns = " << aNVertex << "); valid range is 3 .. 360\n";
            return -1;
        }

        HDRBASE::ns = aNVertex;
    }

    valid = true;

    return 0;
}    // SetParams()


int HDRBASE::calc( void )
{
    if( valid )
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
    }

    POLYGON::x  = NULL;
    POLYGON::y  = NULL;
    POLYGON::z  = NULL;
    POLYGON::nv = 0;

    // check validity of bevel
    if( bev >= xpitch / 3.0 )
    {
        ERRBLURB;
        cerr << "bevel is too large (>= aXPitch/3)\n";
        return -1;
    }

    if( bev >= ypitch * ypins / 3.0 )
    {
        ERRBLURB;
        cerr << "bevel is too large (>= aYPitch*rows/3)\n";
        return -1;
    }

    // check validity of holes
    double thx  = xpitch;
    double thy  = xpitch * ypins;

    if( bev > 0.0 )
    {
        thx -= 2.0 * bev;
        thy -= 2.0 * bev;
    }
    else
    {
        thx -= 2.0 * MIN_CLR;
        thy -= 2.0 * MIN_CLR;
    }

    if( (hd0 > thx) || (hd0 > thy) )
    {
        ERRBLURB;
        cerr << "bottom hole is too large (" << hd0 <<
        "; it may intrude on the bevels or exceed the MIN_CLR specification\n";
        return -1;
    }

    if( (hd1 > thx) || (hd1 > thy) )
    {
        ERRBLURB;
        cerr << "top hole is too large (" << hd1 <<
        "); it may intrude on the bevels or exceed the MIN_CLR specification\n";
        return -1;
    }

    if( bev > 0.0 )
        nv = 6 * xpins + 2;
    else
        nv = 4;

    x = new (nothrow) double[nv * 2];

    if( x == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate memory for vertices";
        nv = 0;
        return -1;
    }

    y = new (nothrow) double[nv * 2];

    if( y == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate memory for vertices";
        nv = 0;
        delete [] x;
        return -1;
    }

    z = new (nothrow) double[nv * 2];

    if( z == NULL )
    {
        ERRBLURB;
        cerr << "could not allocate memory for vertices";
        nv = 0;
        delete [] x;
        delete [] y;
        return -1;
    }

    // calculate vertices for figure-8
    int i;

    if( nv == 4 )
    {
        x[0] = x[4] = -xpitch * xpins / 2.0;
        y[0] = y[4] = -ypitch * ypins / 2.0;
        x[1] = x[5] = -x[0];
        y[1] = y[5] = y[0];
        x[2] = x[6] = x[1];
        y[2] = y[6] = -y[0];
        x[3] = x[7] = x[0];
        y[3] = y[7] = y[2];

        if( sh > 0.0 )
        {
            z[0] = z[1] = z[2] = z[3] = sh;
        }
        else
        {
            z[0] = z[1] = z[2] = z[3] = 0.0;
        }

        z[4] = z[5] = z[6] = z[7] = height;
    }    // if (nv == 4) -- calculating figure-8 vertices
    else
    {
        int nv2 = nv - 1;
        int nv3 = 2 * nv - 1;
        double left = -xpitch * xpins / 2.0;
        double bottom = -ypitch * ypins / 2.0;

        for( i = 0; i < nv / 2; ++i )
        {
            if( sh > 0.0 )
            {
                z[i] = sh;
                z[nv2 - i] = sh;
            }
            else
            {
                z[i] = 0.0;
                z[nv2 - i] = 0.0;
            }

            z[i + nv] = height;
            z[nv3 - i] = height;

            switch( i % 3 )
            {
            case 0:
                x[i] = left;
                y[i] = bottom + bev;
                break;

            case 1:
                x[i] = left + bev;
                y[i] = bottom;
                break;

            case 2:
                left += xpitch;
                x[i] = left - bev;
                y[i] = bottom;
                break;
            }

            x[nv2 - i]  = x[i];
            x[i + nv]   = x[i];
            x[nv3 - i]  = x[i];
            y[nv2 - i]  = -y[i];
            y[i + nv]   = y[i];
            y[nv3 - i]  = -y[i];
        }
    }    // else [if (nv == 4)] -- calculating figure-8 vertices

    // calculate shoulders (if any)
    if( (sh > 0.0) && hassh )
    {
        if( bev > 0.0 )
        {
            // beveled
            sv[0][0] = -xpitch / 2.0 + bev;
            sv[0][1] = -sv[0][0];
            sv[0][3] = -xpitch / 2.0 + 0.2 * bev;
            sv[0][2] = -sv[0][3];

            sv[1][0] = 0.0;
            sv[1][1] = 0.0;

            sv[1][2] = 0.8 * bev;
            sv[1][3] = sv[1][2];
        }
        else
        {
            // no bevel
            sv[0][0] = -xpitch / 2.0 * xpins;
            sv[0][1] = -sv[0][0];
            sv[0][2] = sv[0][1];
            sv[0][3] = sv[0][0];

            sv[1][0] = -ypitch / 2.0 * ypins;
            sv[1][1] = sv[1][0];
            sv[1][2] = sv[1][0] + 0.4;
            sv[1][3] = sv[1][2];
        }

        sv[0][4] = sv[0][0];
        sv[0][5] = sv[0][1];
        sv[0][6] = sv[0][2];
        sv[0][7] = sv[0][3];

        sv[1][4] = sv[1][0];
        sv[1][5] = sv[1][1];
        sv[1][6] = sv[1][2];
        sv[1][7] = sv[1][3];

        sv[2][0] = sv[2][1] = sv[2][2] = sv[2][3] = 0.0;
        sv[2][4] = sv[2][5] = sv[2][6] = sv[2][7] = sh;
    }    // if (sh > 0.0)

    valid = true;
    return 0;
}    // calc()


int HDRBASE::paint( TRANSFORM& t, VRMLMAT& color, bool reuse_color,
        std::ofstream& fp, int tabs )
{
    int val = 0;
    int i, j, k;

    // if beveled, seal the top and bottom polygons
    if( bev > 0.0 )
    {
        double vp[3][8];
        FAKEPOLY pol;
        POLYGON::x  = vp[0];
        POLYGON::y  = vp[1];
        POLYGON::z  = vp[2];
        POLYGON::nv = 4;
        pol.SetParams( &vp[0][4], &vp[1][4], &vp[2][4], 4, true );
        int iter = 0;
        int io = 0;     // index offset
        bool reuse = reuse_color;
        bool top = false;

        while( iter < 2 )
        {
            for( i = 0; i < xpins; ++i )
            {
                j = i * 3;

                for( k = 0; k < 4; ++k )
                {
                    vp[0][k] = x[j + k + io];
                    vp[1][k] = y[j + k + io];
                    vp[2][k] = z[j + k + io];
                    vp[0][7 - k] = x[nv - j - k + io - 1];
                    vp[1][7 - k] = y[nv - j - k + io - 1];
                    vp[2][7 - k] = z[nv - j - k + io - 1];
                }

                val += POLYGON::Paint( top, t, color, reuse, fp, tabs );
                reuse = true;
                val += pol.Paint( top, t, color, true, fp, tabs );
            }

            top = true;
            ++iter;
            io = nv;
        }

        switch( ypins )
        {
        case 1:
            val += makeHoles1( t, color, true, fp, tabs );
            break;

        case 2:
            val += makeHoles2( t, color, true, fp, tabs );
            break;

        default:
            val += makeHoles3( t, color, true, fp, tabs );
            break;
        }

        if( val )
        {
            ERRBLURB;
            cerr << "problems writing to output file\n";
            return -1;
        }

        return 0;
    }    // if (bev > 0.0)

    // frames on the holes are a uniform size
    HOLE bh, th;
    TRANSFORM t0, t1;
    double z0 = 0.0;

    if( sh > 1e-9 )
        z0 = sh;

    double ox, oy;    // offset for locating the hole

    for( i = 0; i < ypins; ++i )
    {
        oy = ( (1 - ypins) / 2.0 + i ) * ypitch;

        for( j = 0; j < xpins; ++j )
        {
            ox = ( (1 - xpins) / 2.0 + j ) * xpitch;
            t0.SetTranslation( ox, oy, z0 );
            t1.SetTranslation( ox, oy, height );
            bh.Calc( xpitch, ypitch, hd0, hdy, t0, squarebot, 0, 0, ns, pbev );

            if( male )
            {
                th.Calc( xpitch, ypitch, hd0, hdy, t1, squarebot, 0, 0, ns, pbev );
            }
            else
            {
                th.Calc( xpitch, ypitch, hd1, hd1, t1, squaretop, 0, 0, ns, fbev );
            }

            val += bh.Build( false, t, color, true, fp, tabs );
            val += th.Build( true, t, color, true, fp, tabs );
        }
    }

    if( val )
    {
        ERRBLURB;
        cerr << "problems writing to output file\n";
        return -1;
    }

    return 0;
}    // Paint()


int HDRBASE::stitch( TRANSFORM& t, VRMLMAT& color, bool reuse_color,
        std::ofstream& fp, int tabs )
{
    FAKEPOLY pol;

    pol.SetParams( &x[nv], &y[nv], &z[nv], nv, true );
    POLYGON::x  = x;
    POLYGON::y  = y;
    POLYGON::z  = z;
    POLYGON::nv = nv;
    int val = 0;
    val += POLYGON::Stitch( true, pol, t, color, reuse_color, fp, tabs );
    POLYGON::x  = NULL;
    POLYGON::y  = NULL;
    POLYGON::z  = NULL;
    POLYGON::nv = 0;

    if( val )
    {
        ERRBLURB;
        cerr << "problems writing out header facets\n";
        return -1;
    }

    // put in an internal rectangular shroud offset from top and bottom by pitch/100 and
    // offset inwards on X by pitch/10 and Y by pitch/50
    double shr[8][4];
    double xp1 = xpitch * 0.02;
    // bottom
    shr[0][0] = x[0] + xp1 * 5;
    shr[0][1] = x[nv / 2 - 1] - xp1 * 5;
    shr[0][2] = x[nv / 2] - xp1 * 5;
    shr[0][3] = x[nv - 1] + xp1 * 5;
    shr[1][0] = y[0] + xp1;
    shr[1][1] = y[nv / 2 - 1] + xp1;
    shr[1][2] = y[nv / 2] - xp1;
    shr[1][3] = y[nv - 1] - xp1;
    shr[2][0] = shr[2][1] = shr[2][2] = shr[2][3] = z[0] + xp1 / 2.0;
    // top
    shr[5][0] = shr[5][1] = shr[5][2] = shr[5][3] = z[nv] - xp1 / 2.0;
    shr[3][0] = shr[0][0];
    shr[3][1] = shr[0][1];
    shr[3][2] = shr[0][2];
    shr[3][3] = shr[0][3];
    shr[4][0] = shr[1][0];
    shr[4][1] = shr[1][1];
    shr[4][2] = shr[1][2];
    shr[4][3] = shr[1][3];
    pol.SetParams( shr[3], shr[4], shr[5], 4, true );
    POLYGON::x  = shr[0];
    POLYGON::y  = shr[1];
    POLYGON::z  = shr[2];
    POLYGON::nv = 4;
    val += POLYGON::Stitch( true, pol, t, color, true, fp, tabs );
    POLYGON::x  = NULL;
    POLYGON::y  = NULL;
    POLYGON::z  = NULL;
    POLYGON::nv = 0;

    if( val )
    {
        ERRBLURB;
        cerr << "problems writing out shroud facets\n";
        return -1;
    }


    if( !hassh )
        return 0;

    // render the shoulders
    double tsv[3][8];
    POLYGON::x  = tsv[0];
    POLYGON::y  = tsv[1];
    POLYGON::z  = tsv[2];
    POLYGON::nv = 4;
    pol.SetParams( &tsv[0][4], &tsv[1][4], &tsv[2][4], 4, true );
    TRANSFORM t0;
    double xo, yo;    // X and Y offset
    xo  = -(xpins - 1) / 2.0 * xpitch;
    yo  = -ypitch * ypins / 2.0;
    int i, j, k;

    if( bev > 0.0 )
    {
        k = 0;

        while( k < 2 )
        {
            for( i = 0; i < xpins; ++i )
            {
                t0.SetTranslation( xo + i * xpitch, yo, 0 );

                for( j = 0; j < 8; ++j )
                {
                    tsv[0][j] = sv[0][j];
                    tsv[1][j] = sv[1][j];
                    tsv[2][j] = sv[2][j];
                }

                t0.Transform( tsv[0], tsv[1], tsv[2], 8 );
                val += POLYGON::Stitch( true, pol, t, color, true, fp, tabs );
                // paint the ends as well
                val += POLYGON::Paint( false, t, color, true, fp, tabs );
            }

            ++k;
            yo = -yo;
            t0.SetRotation( M_PI, 0, 0, 1 );
        }
    }
    else
    {
        for( i = 0; i < 8; ++i )
        {
            tsv[0][i] = sv[0][i];
            tsv[1][i] = sv[1][i];
            tsv[2][i] = sv[2][i];
        }

        val += POLYGON::Stitch( true, pol, t, color, true, fp, tabs );
        POLYGON::Paint( false, t, color, true, fp, tabs );
        pol.Paint( true, t, color, true, fp, tabs );
        t0.SetRotation( M_PI, 0, 0, 1 );
        t0.Transform( tsv[0], tsv[1], tsv[2], 8 );
        val += POLYGON::Stitch( true, pol, t, color, true, fp, tabs );
        val += POLYGON::Paint( false, t, color, true, fp, tabs );
    }

    POLYGON::x  = NULL;
    POLYGON::y  = NULL;
    POLYGON::z  = NULL;
    POLYGON::nv = 0;

    if( val )
    {
        ERRBLURB;
        cerr << "problems writing out header facets\n";
        return -1;
    }

    return 0;
}    // Stitch()


int HDRBASE::makeHoles1( TRANSFORM& t, VRMLMAT& color, bool reuse_color,
        std::ofstream& fp, int tabs )
{
    // frames on the holes are a uniform size
    HOLE bh, th;
    TRANSFORM t0, t1;
    int i;
    int val = 0;
    double z0 = 0.0;

    if( sh > 1e-9 )
        z0 = sh;

    double ox;  // offset for locating the hole
    bool reuse = reuse_color;

    for( i = 0; i < xpins; ++i )
    {
        ox = ( (1 - xpins) / 2.0 + i ) * xpitch;
        t0.SetTranslation( ox, 0, z0 );
        t1.SetTranslation( ox, 0, height );
        bh.Calc( xpitch, ypitch - 2.0 * bev, hd0, hdy, t0, squarebot, 0, 0, ns, pbev );

        if( male )
        {
            th.Calc( xpitch, ypitch - 2.0 * bev, hd0, hdy, t1, squarebot, 0, 0, ns, pbev );
        }
        else
        {
            th.Calc( xpitch, ypitch - 2.0 * bev, hd1, hd1, t1, squaretop, 0, 0, ns, fbev );
        }

        val += bh.Build( false, t, color, reuse, fp, tabs );
        reuse = true;
        val += th.Build( true, t, color, true, fp, tabs );
    }

    if( val )
    {
        ERRBLURB;
        cerr << "problems writing to output file\n";
        return -1;
    }

    return 0;
}


int HDRBASE::makeHoles2( TRANSFORM& t, VRMLMAT& color, bool reuse_color,
        std::ofstream& fp, int tabs )
{
    // frames on the holes are a uniform size but have different hole offsets
    HOLE bh, th;
    TRANSFORM t0, t1;
    int i, j;
    int val = 0;
    double z0 = 0.0;

    if( sh > 1e-9 )
        z0 = sh;

    double ox, oy;  // offsets for locating the hole
    double ho;      // hole offset (Y axis only)
    oy  = -ypitch / 2.0 + bev / 2.0;
    ho  = -bev / 2.0;
    j   = 0;
    bool reuse = reuse_color;

    while( j < 2 )
    {
        for( i = 0; i < xpins; ++i )
        {
            ox = ( (1 - xpins) / 2.0 + i ) * xpitch;
            t0.SetTranslation( ox, oy, z0 );
            t1.SetTranslation( ox, oy, height );
            bh.Calc( xpitch, ypitch - bev, hd0, hdy, t0, squarebot, 0, ho, ns, pbev );

            if( male )
            {
                th.Calc( xpitch, ypitch - bev, hd0, hdy, t1, squarebot, 0, ho, ns, pbev );
            }
            else
            {
                th.Calc( xpitch, ypitch - bev, hd1, hd1, t1, squaretop, 0, ho, ns, fbev );
            }

            val += bh.Build( false, t, color, reuse, fp, tabs );
            reuse = true;
            val += th.Build( true, t, color, true, fp, tabs );
        }

        ho  = -ho;
        oy  = -oy;
        ++j;
    }

    if( val )
    {
        ERRBLURB;
        cerr << "problems writing to output file\n";
        return -1;
    }

    return 0;
}


int HDRBASE::makeHoles3( TRANSFORM& t, VRMLMAT& color, bool reuse_color,
        std::ofstream& fp, int tabs )
{
    // frames on the holes are one size at front and back size but have different hole offsets
    // while holes in the middle are all the same
    HOLE bh, th;
    TRANSFORM t0, t1;
    int i, j;
    int val = 0;
    double z0 = 0.0;

    if( sh > 1e-9 )
        z0 = sh;

    double ox, oy;    // offsets for locating the hole
    oy  = (1 - ypins) * ypitch / 2.0 + bev / 2.0;
    j   = 0;
    bool reuse = reuse_color;

    while( j < 2 )
    {
        for( i = 0; i < xpins; ++i )
        {
            ox = ( (1 - xpins) / 2.0 + i ) * xpitch;
            t0.SetTranslation( ox, oy, z0 );
            t1.SetTranslation( ox, oy, height );
            bh.Calc( xpitch, ypitch - bev, hd0, hdy, t0, squarebot, 0, 0, ns, pbev );

            if( male )
            {
                th.Calc( xpitch, ypitch - bev, hd0, hdy, t1, squarebot, 0, 0, ns, pbev );
            }
            else
            {
                th.Calc( xpitch, ypitch - bev, hd1, hd1, t1, squaretop, 0, 0, ns, fbev );
            }

            val += bh.Build( false, t, color, reuse, fp, tabs );
            reuse = true;
            val += th.Build( true, t, color, true, fp, tabs );
        }

        oy = -oy;
        ++j;
    }

    for( i = 1; i < ypins - 1; ++i )
    {
        oy = ( (1 - ypins) / 2.0 + i ) * ypitch;

        for( j = 0; j < xpins; ++j )
        {
            ox = ( (1 - xpins) / 2.0 + j ) * xpitch;
            t0.SetTranslation( ox, oy, z0 );
            t1.SetTranslation( ox, oy, height );
            bh.Calc( xpitch, ypitch, hd0, hdy, t0, squarebot, 0, 0, ns, pbev );

            if( male )
            {
                th.Calc( xpitch, ypitch, hd0, hdy, t1, squarebot, 0, 0, ns, pbev );
            }
            else
            {
                th.Calc( xpitch, ypitch, hd1, hd1, t1, squaretop, 0, 0, ns, fbev );
            }

            val += bh.Build( false, t, color, true, fp, tabs );
            val += th.Build( true, t, color, true, fp, tabs );
        }
    }

    if( val )
    {
        ERRBLURB;
        cerr << "problems writing to output file\n";
        return -1;
    }

    return 0;
}


// Write the header shape to an output file
int HDRBASE::Build( KC3D::TRANSFORM& aTransform, KC3D::VRMLMAT& aMaterial, bool reuseMaterial,
                    std::ofstream& aVRMLFile, int aTabDepth )
{
    if( !valid )
    {
        ERRBLURB;
        cerr << "invoked without prior invocation of SetParams()\n";
        return -1;
    }

    int val = 0;
    val += calc();
    val += stitch( aTransform, aMaterial, reuseMaterial, aVRMLFile, aTabDepth );
    val += paint( aTransform, aMaterial, true, aVRMLFile, aTabDepth );

    if( val )
    {
        ERRBLURB;
        cerr << "problems writing data to file\n";
        return -1;
    }

    return 0;
}


/* Inherited (overridden) Paint */
int HDRBASE::Paint( bool ccw, TRANSFORM& t, VRMLMAT& color, bool reuse_color,
        std::ofstream& fp, int tabs )
{
    ERRBLURB;
    cerr << "BUG: unexpected code execution branch\n";
    return -1;
}


/* Inherited (overridden) Calc */
int HDRBASE::Calc( double, double, KC3D::TRANSFORM& )
{
    ERRBLURB;
    cerr << "BUG: unexpected code execution branch\n";
    return -1;
}


/* Inherited (overridden) Stitch */
int HDRBASE::Stitch( bool ccw, POLYGON& p2, TRANSFORM& t,
        VRMLMAT& color, bool reuse_color, std::ofstream& fp, int tabs )
{
    ERRBLURB;
    cerr << "BUG: unexpected code execution branch\n";
    return -1;
}
