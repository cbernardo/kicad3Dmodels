/*
 *      file: hole.cpp
 *
 *      Copyright 2012 Dr. Cirilo Bernardo (cjh.bernardo@gmail.com)
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

#include "hole.h"
#include "vdefs.h"
#include "vcom.h"
#include "transform.h"
#include "polygon.h"
#include "circle.h"
#include "rectangle.h"

using namespace std;
using namespace kc3d;

Hole::Hole()
{
    valid = false;
    hole = NULL;
    np = 16;
    return;
}

Hole::~Hole()
{
    if (hole) delete hole;
    return;
}

int Hole::Calc(double w1, double d1, double w2, double d2, Transform &tx,
        bool square, double ow, double od, int np)
{
    if (valid)
    {
        valid = false;
        delete hole;
        hole = NULL;
    }

    // validate the parameters
    if (w1 <= 0)
    {
        ERRBLURB;
        cerr << "invalid w1: <= 0\n";
        return -1;
    }
    if ((w2 <= 0) || (w2 >= w1))
    {
        ERRBLURB;
        cerr << "invalid w2: <= 0 or >= w1\n";
        return -1;
    }
    if (((w2/2.0 + ow) >= w1/2.0) || ((w2/2.0 - ow) >= w1/2.0))
    {
        ERRBLURB;
        cerr << "invalid parameters: w2/2.0 + |ow| >= w1/2.0\n";
        return -1;
    }
    if (d1 <= 0)
    {
        ERRBLURB;
        cerr << "invalid d1: <= 0\n";
        return -1;
    }
    if ((d2 <= 0) || (d2 >= d1))
    {
        ERRBLURB;
        cerr << "invalid d2: <= 0 or >= d1\n";
        return -1;
    }
    if (((d2/2.0 + od) >= d1/2.0) || ((d2/2.0 - od) >= d1/2.0))
    {
        ERRBLURB;
        cerr << "invalid parameters: d2/2.0 + |od| >= d1/2.0\n";
        return -1;
    }
    if ((!square) && ((np < 4) || (np > 360) || (np % 4)))
    {
        ERRBLURB;
        cerr << "invalid np: must be 4 .. 360 and a multiple of 4\n";
        cerr << "\tUsing default 16\n";
        np = 16;
    }
    Hole::square = square;

    // calculate the frame
    double tw, td;
    tw = w1/2.0;
    td = d1/2.0;
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
    Transform t0;
    t0.setTranslation(ow, od, 0.0);
    if (square)
    {
        hole = new (nothrow) Rectangle;
    }
    else
    {
        hole = new (nothrow) Circle(np);
    }
    if (!hole)
    {
        ERRBLURB;
        cerr << "could not allocate memory for hole\n";
        return -1;
    }
    hole->Calc(w2, d2, t0);

    tx.transform(v[0], v[1], v[2], 4);
    hole->Xform(tx);

    valid = true;
    return 0;
}


int Hole::Build(bool top, Transform &t, VRMLMat &color, bool reuse,
        std::ofstream &fp, int tabs)
{
    if (!valid)
    {
        ERRBLURB;
        cerr << "invoked with no prior successful call to Calc()\n";
        return -1;
    }

    double *lx, *ly, *lz;
    int np = hole->GetVertices(&lx, &ly, &lz);
    int np2 = np + 4;

    double *tx, *ty, *tz;

    tx = new (nothrow) double [np2];
    if (!tx)
    {
        ERRBLURB;
        cerr << "cannot allocate memory for vertices\n";
        return -1;
    }

    ty = new (nothrow) double [np2];
    if (!ty)
    {
        ERRBLURB;
        cerr << "cannot allocate memory for vertices\n";
        delete [] tx;
        return -1;
    }

    tz = new (nothrow) double [np2];
    if (!tz)
    {
        ERRBLURB;
        cerr << "cannot allocate memory for vertices\n";
        delete [] tx;
        delete [] ty;
        return -1;
    }

    int i, idx;
    for (i = 0; i < 4; ++i)
    {
        tx[i] = v[0][i];
        ty[i] = v[1][i];
        tz[i] = v[2][i];
    }
    idx = 0;
    for (i = 4; i < np2; ++i, ++idx)
    {
        tx[i] = lx[idx];
        ty[i] = ly[idx];
        tz[i] = lz[idx];
    }
    t.transform(tx, ty, tz, np2);

    int acc = 0;

    // set up VRML Shape
    acc += SetupShape(color, reuse, fp, tabs);

    // enumerate vertices
    acc += WriteCoord(tx, ty, tz, np2, fp, tabs +1);
    delete [] tx;
    delete [] ty;
    delete [] tz;

    // enumerate facets
    acc += SetupCoordIndex(fp, tabs +1);
    if (square)
    {
        acc += writeRFacets(top, fp, tabs +2);
    }
    else
    {
        acc += writeFacets(top, 0, 1, 4+np*3/4, 3+np, 4, fp, tabs +2);
        acc += writeFacets(top, 1, 2, 4, 3+np/4, 4+np/4, fp, tabs +2);
        acc += writeFacets(top, 2, 3, 4+np/4, 3+np/2, 4+np/2, fp, tabs +2);
        acc += writeFacets(top, 3, 0, 4+np/2, 3+np*3/4, 4+np*3/4, fp, tabs +2, true);
    }
    acc += CloseCoordIndex(fp, tabs +1);
    acc += CloseShape(fp, tabs);

    if (acc)
    {
        ERRBLURB;
        cerr << "problems writing data\n";
        return -1;
    }
    return 0;
}


int Hole::writeFacets(bool top, int v0, int v1, int h0, int h1, int lp,
        std::ofstream &fp, int tabs, bool term)
{
    int i;

    string fmt(tabs*4, ' ');
    if (top)
    {
        // render the top
        fp << fmt;
        for (i = h0; i < h1; ++i)
        {
            fp << i << "," << v0 << "," << i+1 << ",-1,";
            if (!((i + 1) % 18))
                fp << "\n" << fmt;
        }
        fp << h1 << "," << v0 << "," << lp << ",-1,\n";
        // large triangular facet
        if (term)
            fp << fmt << v0 << "," << v1 << "," << lp << ",-1\n";
        else
            fp << fmt << v0 << "," << v1 << "," << lp << ",-1,\n";
    }
    else
    {
        // render the bottom
        fp << fmt;
        for (i = h0; i < h1; ++i)
        {
            fp << v0 << "," << i << "," << i+1 << ",-1,";
            if (!((i + 1) % 18))
                fp << "\n" << fmt;
        }
        fp << v0 << "," << h1 << "," << lp << ",-1,\n";
        // large triangular facet
        if (term)
            fp << fmt << v1 << "," << v0 << "," << lp << ",-1\n";
        else
            fp << fmt << v1 << "," << v0 << "," << lp << ",-1,\n";
    }

    if (!fp.good()) return -1;
    return 0;
}

int Hole::writeRFacets(bool top, std::ofstream &fp, int tabs)
{
    string fmt(tabs*4, ' ');
    if (top)
    {
        fp << fmt;
        fp << "0,1,5,4,-1,1,2,6,5,-1,2,3,7,6,-1,3,0,4,7,-1,\n";
    }
    else
    {
        fp << fmt;
        fp << "1,0,4,5,-1,2,1,5,6,-1,3,2,6,7,-1,0,3,7,4,-1\n";
    }

    if (!fp.good()) return -1;
    return 0;
}
