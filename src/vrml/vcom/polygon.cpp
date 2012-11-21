/*
 *      file: polygon.cpp
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
#include <iomanip>
#include <new>

#include "vdefs.h"
#include "vcom.h"
#include "polygon.h"
#include "transform.h"
#include "vrmlmat.h"


using namespace std;
using namespace kc3d;

Polygon::Polygon()
{
    nv = 0;
    init();
    return;
}

Polygon::~Polygon()
{
    if (x) delete [] x;
    if (y) delete [] y;
    if (z) delete [] z;
    init();
    return;
}

bool Polygon::IsValid(void) const
{
    return valid;
}

void Polygon::init(void)
{
    x = y = z = NULL;
    valid = false;
    return;
}


int Polygon::Paint(bool ccw, Transform &t, VRMLMat &color, bool reuse_color,
        std::ofstream &fp, int tabs)
{
    int i;

    if (!valid)
    {
        ERRBLURB;
        cerr << "invoked without prior invocation of Calc()\n";
        return -1;
    }

    if (nv < 3)
    {
        ERRBLURB;
        cerr << "invalid number of vertices (" << nv << "); range is 3 .. 360\n";
        return -1;
    }

    if (tabs < 0) tabs = 0;
    if (tabs > 4) tabs = 4;

    double *lx, *ly, *lz;

    // allocate memory
    lx = new (nothrow) double [nv];
    if (lx == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for intermediate calculations\n";
        return -1;
    }
    ly = new (nothrow) double [nv];
    if (ly == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for intermediate calculations\n";
        delete [] lx;
        return -1;
    }
    lz = new (nothrow) double [nv];
    if (lz == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for intermediate calculations\n";
        delete [] lx;
        delete [] ly;
        return -1;
    }

    for (i = 0; i < nv; ++i)
    {
        lx[i] = x[i];
        ly[i] = y[i];
        lz[i] = z[i];
    }

    // perform transforms
    t.transform(lx, ly, lz, nv);

    // set up VRML Shape
    SetupShape(color, reuse_color, fp, tabs);
    // enumerate vertices
    WriteCoord(lx, ly, lz, nv, fp, tabs +1);
    // enumerate facets
    SetupCoordIndex(fp, tabs +1);
    string fmt((tabs + 1)*4, ' ');
    fp << fmt << "    ";
    if (ccw)
    {
        for (i = 0; i < nv-1; ++i)
        {
            fp << i << ",";
            if (!((i + 1) % 18))
                fp << "\n" << fmt << "    ";
        }
        fp << i << ",0,-1\n";
    }
    else
    {
        for (i = nv-1; i > 0; --i)
        {
            fp << i << ",";
            if (!((i + 1) % 18))
                fp << "\n" << fmt << "    ";
        }
        fp << "0," << nv-1 << ",-1\n";
    }
    CloseCoordIndex(fp, tabs +1);

    delete [] lx;
    delete [] ly;
    delete [] lz;
    return CloseShape(fp, tabs);
}


int Polygon::Stitch(bool ccw, Polygon &p2, Transform &t,
            VRMLMat &color, bool reuse_color, std::ofstream &fp, int tabs)
{
    int i, j, k;

    if (!valid)
    {
        ERRBLURB;
        cerr << "invoked without prior invocation of Calc()\n";
        return -1;
    }

    if (nv < 3)
    {
        ERRBLURB;
        cerr << "invalid number of vertices (" << nv << "); range is 3 .. 360\n";
        return -1;
    }

    double *r2x = NULL;
    double *r2y = NULL;
    double *r2z = NULL;
    int rv = p2.GetVertices(&r2x, &r2y, &r2z);
    if (rv != nv)
    {
        ERRBLURB;
        cerr << "points in second ring (" << rv << ") do not match points in this ring (" << nv << ")\n";
        return -1;
    }

    if (tabs < 0) tabs = 0;
    if (tabs > 4) tabs = 4;

    double *lx, *ly, *lz;
    lx = ly = lz = NULL;

    // allocate memory
    lx = new double[nv*2];
    if (lx == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for intermediate calculations\n";
        return -1;
    }
    ly = new double[nv*2];
    if (ly == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for intermediate calculations\n";
        delete [] lx;
        return -1;
    }
    lz = new double[nv*2];
    if (lz == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for intermediate calculations\n";
        delete [] lx;
        delete [] ly;
        return -1;
    }

    for (i = 0; i < nv; ++i)
    {
        lx[i] = x[i];
        ly[i] = y[i];
        lz[i] = z[i];
        lx[i+nv] = r2x[i];
        ly[i+nv] = r2y[i];
        lz[i+nv] = r2z[i];
    }

    // perform transforms
    t.transform(lx, ly, lz, nv*2);

    // set up VRML Shape
    SetupShape(color, reuse_color, fp, tabs);
    // enumerate vertices
    WriteCoord(lx, ly, lz, nv*2, fp, tabs +1);
    // enumerate facets
    SetupCoordIndex(fp, tabs +1);
    string fmt((tabs + 1)*4, ' ');
    fp << fmt << "   ";
    if (ccw)
        k = 1;
    else
        k = -1;
    for (i = 0; i < nv-1; ++i)
    {
        j = i + k;
        if (j >= nv) j -= nv;
        if (j < 0) j += nv;
        fp << " " << i << "," << j << "," <<  j+nv << "," << i+nv << ",-1,";
        if (!((i + 1) % 4))
            fp << "\n" << fmt << "   ";
    }
    j = i + k;
    if (j >= nv) j -= nv;
    if (j < 0) j += nv;
    fp << " " << i << "," << j << "," <<  j+nv << "," << i+nv << ",-1\n";

    CloseCoordIndex(fp, tabs +1);

    delete [] lx;
    delete [] ly;
    delete [] lz;
    return CloseShape(fp, tabs);
}


// Transform all points in the polygon
int Polygon::Xform(Transform &T)
{
    if (!valid)
    {
        ERRBLURB;
        cerr << "invoked without prior invocation of Calc()\n";
        return -1;
    }
    T.transform(x, y, z, nv);
    return 0;
}


// Return value: number of points. Handles will point to arrays of doubles
int Polygon::GetVertices(double **px, double **py, double **pz) const
{
    *px = x;
    *py = y;
    *pz = z;
    return nv;
}


int Polygon::Extrude(bool cap0, bool cap1, bool outer, Quat &center, Transform &upto, Transform &txout,
        VRMLMat &color, bool reuse, std::ofstream &fp, int tabs)
{
    SimplePoly pp(*this);
    if (!pp.IsValid())
    {
        ERRBLURB;
        cerr << "could not duplicate polygon\n";
        return -1;
    }
    Transform tx;
    tx.setTranslation(-center);
    // move the shape back to the global origin
    int acc = pp.Xform(tx);
    // transform the shape
    acc += pp.Xform(upto);
    // shift the shape back
    tx.setTranslation(center);
    acc += pp.Xform(tx);

    if (outer)
    {
       acc += this->Stitch(true, pp, txout, color, reuse, fp, tabs);
       if (cap0) acc += this->Paint(false, txout, color, true, fp, tabs);
       if (cap1) acc += pp.Paint(true, txout, color, true, fp, tabs);
    }
    else
    {
        acc += this->Stitch(false, pp, txout, color, reuse, fp, tabs);
        if (cap0) acc += this->Paint(true, txout, color, true, fp, tabs);
        if (cap1) acc += pp.Paint(false, txout, color, true, fp, tabs);
    }

    if (acc)
    {
        ERRBLURB;
        cerr << "problems creating the extrusion\n";
        return -1;
    }
    return 0;
}





Polygon *FakePoly::clone(void) const
{
    ERRBLURB;
    cerr << "BUG: FakePoly cannot be cloned, it must not be treated as a derived Polygon\n";
    return NULL;
}

int FakePoly::Calc(double xl, double yl, Transform &t)
{
    ERRBLURB;
    cerr << "BUG: FakePoly does not support Calc(), it must not be treated as a derived Polygon\n";
    return -1;
}

FakePoly::~FakePoly()
{
    // Do not allow the Polygon destructor to free memory
    Polygon::x = NULL;
    Polygon::y = NULL;
    Polygon::z = NULL;
    Polygon::nv = 0;
    Polygon::valid = false;
}


void FakePoly::setParams(double *x, double *y, double *z, int np, bool valid)
{
    Polygon::x = x;
    Polygon::y = y;
    Polygon::z = z;
    Polygon::nv = np;
    Polygon::valid = valid;
    return;
}




SimplePoly::SimplePoly()
{
    return;
}

SimplePoly::~SimplePoly()
{
    return;
}

SimplePoly::SimplePoly(const Polygon &p)
{
    if (SetValues(p)) Polygon::init();
    return;
}

SimplePoly::SimplePoly(const SimplePoly &p)
{
    if (SetValues(p)) Polygon::init();
    return;
}

// inherited methods
Polygon *SimplePoly::clone(void) const
{
    ERRBLURB;
    cerr << "BUG: SimplePoly cannot be cloned, it must not be treated as a derived Polygon\n";
    return NULL;
}

int SimplePoly::Calc(double xl, double yl, Transform &t)
{
    ERRBLURB;
    cerr << "BUG: SimplePoly does not support Calc(), it must not be treated as a derived Polygon\n";
    return -1;
}

// extended methods
int SimplePoly::SetValues(const Polygon &p)
{
    if (x) delete [] x;
    if (y) delete [] y;
    if (z) delete [] z;
    x = y = z = NULL;
    nv = 0;
    valid = false;

    int np;
    double *px, *py, *pz;
    px = py = pz = NULL;
    np = p.GetVertices(&px, &py, &pz);
    if (np < 3) return 0; // polygons must have 3 or more vertices
    if ((!px) || (!py) || (!pz))
    {
        ERRBLURB;
        cerr << "invalid pointer to coordinate\n";
        return -1;
    }

    x = new (nothrow) double [np];
    if (x == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate memory for vertices\n";
        return -1;
    }
    y = new (nothrow) double [np];
    if (y == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate memory for vertices\n";
        delete [] x;
        return -1;
    }
    z = new (nothrow) double [np];
    if (z == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate memory for vertices\n";
        delete [] x;
        delete [] y;
        return -1;
    }
    int i;
    for (i = 0; i < np; ++i)
    {
        x[i] = px[i];
        y[i] = py[i];
        z[i] = pz[i];
    }
    valid = true;
    nv = np;
    return 0;
}
