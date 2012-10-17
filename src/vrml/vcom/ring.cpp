/*
 *      file: ring.cpp
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
#include "ring.h"
#include "transform.h"
#include "vrmlmat.h"

using namespace std;

#define MIN_RAD (0.01)
#define MAX_RAD (100.0)

Ring::Ring()
{
    init();
    return;
}

Ring::~Ring()
{
    if (x) delete [] x;
    if (y) delete [] y;
    if (z) delete [] z;
    init();
    return;
}

Ring::init()
{
    cx = cy = cz = 0.0;
    x = y = z = NULL;
    np = 0;
    valid = false;
    return;
}


int Ring::Calc(int np, double xrad, double yrad, const Transform &t)
{
    if (valid)
    {
        if (x) delete [] x;
        if (y) delete [] y;
        if (z) delete [] z;
        init();
    }

    if ((np < 8)||(np > 360)||(np % 4))
    {
        ERRBLURB;
        cerr << "Invalid number of points (" << np << "). Range is 8..360 in multiples of 4.\n";
        return -1;
    }

    if ((xrad < MIN_RAD)||(xrad > MAX_RAD))
    {
        ERRBLURB;
        cerr << "Invalid X radius (" << xrad << "). Range is " << MIN_RAD << " to " << MAX_RAD "\n";
        return -1;
    }

    if ((yrad < MIN_RAD)||(yrad > MAX_RAD))
    {
        ERRBLURB;
        cerr << "Invalid Y radius (" << yrad << "). Range is " << MIN_RAD << " to " << MAX_RAD "\n";
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
    double da = M_PI/np;
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
    // transform the center
    t.transform(&cx, &cy, &cz, 1);

    valid = true;
    return 0;
}   // Calc



int Ring::Render(const std::string &name, bool forward, const Transform &t,
            const VRMLMat &color, bool reuse_color, ofstream &fp, int tabs = 0)
{
    if (!valid)
    {
        ERRBLOB;
        cerr << "invoked without prior invocation of Calc()\n";
        return -1;
    }

    if (name.empty())
    {
        ERRBLOB;
        cerr << "empty name\n";
        return -1;
    }

    if (tabs < 0) tabs = 0;
    if (tabs > 4) tabs = 4;
    string fmt(tabs*4, ' ');

    double lcx = cx;
    double lcy = cy;
    double lcz = cz;
    double *lx, *ly, *lz;
    lx = ly = lz = NULL;

    // allocate memory
    lx = new double[np];
    if (lx == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for intermediate calculations\n";
        return -1;
    }
    ly = new double[np];
    if (ly == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for intermediate calculations\n";
        delete [] lx;
        return -1;
    }
    lz = new double[np];
    if (lz == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for intermediate calculations\n";
        delete [] lx;
        delete [] ly;
        return -1;
    }

    int i;
    for (i = 0; i < np; ++i)
    {
        lx[i] = x[i];
        ly[i] = y[i];
        lz[i] = z[i];
    }
    lcx = cx;
    lcy = cy;
    lcz = cz;

    // perform transforms
    t.transform(lx, ly, lz, np);
    t.transform(&cx, &cy, &cz, 1);

    // set up VRML Transform environment
    SetupXForm(name, color, reuse_color, fp, tabs)
    // XXX - set up appearance
    // XXX - enumerate vertices
    // XXX - enumerate facets
    // XXX - close transform
}

    // Return values: 0 for success otherwise -1
    /// @param name Name for the VRML Transform entity
    /// @param ring The ring to stitch to
    /// @param forward Determines order of vertices
    /// @param t Transform to reorient the ellipse
    /// @param color Material appearance to use
    /// @param reuse_color Reuse (true) or define an appearance
    /// @param fp The output stream
    /// @param tabs The tabulator level (max. 4)
    int Stitch(const std::string &name, const Ring &ring, bool forward, const Transform &t,
            const VRMLMat &color, bool reuse_color, ofstream &fp, int tabs = 0);

// Return value: number of points. Handles will point to arrays of doubles
int GetVertices(double **px, double **py, double **pz)
{
    *px = x;
    *py = y;
    *pz = z;
    return np;
}
