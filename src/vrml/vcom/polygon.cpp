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

#include "vdefs.h"
#include "vcom.h"
#include "polygon.h"
#include "transform.h"
#include "vrmlmat.h"


using namespace std;

Polygon::Polygon()
{
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

void Polygon::init(void)
{
    x = y = z = NULL;
    nv = 0;
    valid = false;
    return;
}

#define MIN_RAD (0.000001)
#define MAX_RAD (100.0)



int Polygon::Calc(int np, double xrad, double yrad, Transform &t)
{
    if (valid)
    {
        if (x) delete [] x;
        if (y) delete [] y;
        if (z) delete [] z;
        init();
    }

    if ((np < 3)||(np > 360))
    {
        ERRBLURB;
        cerr << "Invalid number of points (" << np << "). Range is 3..360 in multiples of 4.\n";
        return -1;
    }

    if ((xrad < MIN_RAD)||(xrad > MAX_RAD))
    {
        ERRBLURB;
        cerr << "Invalid X radius (" << xrad << "). Range is " << MIN_RAD << " to " << MAX_RAD << "\n";
        return -1;
    }

    if ((yrad < MIN_RAD)||(yrad > MAX_RAD))
    {
        ERRBLURB;
        cerr << "Invalid Y radius (" << yrad << "). Range is " << MIN_RAD << " to " << MAX_RAD << "\n";
        return -1;
    }

    x = new double[np];
    if (x == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for vertices\n";
        init();
        return -1;
    }
    y = new double[np];
    if (y == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for vertices\n";
        delete [] x;
        init();
        return -1;
    }
    z = new double[np];
    if (z == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for vertices\n";
        delete [] x;
        delete [] y;
        init();
        return -1;
    }

    // calculate the vertices then apply the transform
    double da = M_PI/np*2.0;
    double ang = 0.0;
    int i;

    for (i = 0; i < np; ++ i)
    {
        x[i] = xrad*cos(ang);
        y[i] = yrad*sin(ang);
        z[i] = 0.0;
        ang += da;
    }

    // transform the vertices
    t.transform(x, y, z, np);

    valid = true;
    nv = np;
    return 0;
}   // Calc

int Polygon::Paint(bool ccw, Transform &t, VRMLMat &color, bool reuse_color,
        std::ofstream &fp, int tabs)
{
    int i, j, k;

    if (!valid)
    {
        ERRBLURB;
        cerr << "invoked without prior invocation of Calc()\n";
        return -1;
    }

    if (tabs < 0) tabs = 0;
    if (tabs > 4) tabs = 4;

    double *lx, *ly, *lz;
    lx = ly = lz = NULL;

    // allocate memory
    lx = new double[nv];
    if (lx == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for intermediate calculations\n";
        return -1;
    }
    ly = new double[nv];
    if (ly == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for intermediate calculations\n";
        delete [] lx;
        return -1;
    }
    lz = new double[nv];
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
    t.transform(lx, ly, lz, nv + 1);

    // set up VRML Shape
    SetupShape(color, reuse_color, fp, tabs);
    // enumerate vertices
    WriteCoord(lx, ly, lz, nv +1, fp, tabs +1);
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


int Polygon::Stitch(Polygon &p2, bool ccw, Transform &t,
            VRMLMat &color, bool reuse_color, std::ofstream &fp, int tabs)
{
    int i, j, k;

    if (!valid)
    {
        ERRBLURB;
        cerr << "invoked without prior invocation of Calc()\n";
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

// Return value: number of points. Handles will point to arrays of doubles
int Polygon::GetVertices(double **px, double **py, double **pz)
{
    *px = x;
    *py = y;
    *pz = z;
    return nv;
}

