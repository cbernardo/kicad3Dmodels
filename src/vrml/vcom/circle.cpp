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
#include "circle.h"
#include "rectangle.h"
#include "transform.h"
#include "vrmlmat.h"


using namespace std;
using namespace kc3d;

Circle::Circle()
{
    Polygon::init();
    nv = 16;
    return;
}

Circle::Circle(int vertices)
{
    Polygon::init();
    if ((vertices < 3) || (vertices > 360))
    {
	ERRBLURB;
	cerr << "invalid vertices (" << vertices << "); range is 3 .. 360\n";
	nv = 16;
	return;
    }
    nv = vertices;
    return;
}


Circle::Circle(const Circle &p)
{
    valid = p.valid;
    nv = p.nv;

    x = NULL;
    y = NULL;
    z = NULL;

    if (!valid) return;

    x = new (nothrow) double [nv];
    if (x == NULL)
    {
        valid = false;
        return;
    }

    y = new (nothrow) double [nv];
    if (y == NULL)
    {
        delete [] x;
        valid = false;
        return;
    }

    z = new (nothrow) double [nv];
    if (z == NULL)
    {
        delete [] x;
        delete [] y;
        valid = false;
        return;
    }

    int i;
    for (i = 0; i < nv; ++i)
    {
        x[i] = p.x[i];
        y[i] = p.y[i];
        z[i] = p.z[i];
    }

    return;
}


Circle & Circle::operator=(const Circle &p)
{
    if (this == &p) return *this;

    if (valid)
    {
        delete [] x;
        delete [] y;
        delete [] z;
        nv = 0;
        valid = false;
    }

    valid = p.valid;
    nv = p.nv;

    x = NULL;
    y = NULL;
    z = NULL;

    if (!valid) return *this;

    x = new (nothrow) double [nv];
    if (x == NULL)
    {
        valid = false;
        return *this;
    }

    y = new (nothrow) double [nv];
    if (y == NULL)
    {
        delete [] x;
        valid = false;
        return *this;
    }

    z = new (nothrow) double [nv];
    if (z == NULL)
    {
        delete [] x;
        delete [] y;
        valid = false;
        return *this;
    }

    int i;
    for (i = 0; i < nv; ++i)
    {
        x[i] = p.x[i];
        y[i] = p.y[i];
        z[i] = p.z[i];
    }

    return *this;
}

// Clone the object
Polygon *Circle::clone(void)
{
    return new (nothrow) Circle(*this);
}


Circle::~Circle()
{
    if (x) delete [] x;
    if (y) delete [] y;
    if (z) delete [] z;
    Polygon::init();
    return;
}

#define MIN_RAD (0.000001)
#define MAX_RAD (100.0)



int Circle::Calc(double xdia, double ydia, Transform &t)
{
    if (valid)
    {
        if (x) delete [] x;
        if (y) delete [] y;
        if (z) delete [] z;
        Polygon::init();
        valid = false;
    }
    double xrad, yrad;
    xrad = xdia/2.0;
    yrad = ydia/2.0;

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

    x = new (nothrow) double[nv];
    if (x == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for vertices\n";
	Polygon::init();
        return -1;
    }
    y = new (nothrow) double[nv];
    if (y == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for vertices\n";
        delete [] x;
	Polygon::init();
        return -1;
    }
    z = new (nothrow) double[nv];
    if (z == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for vertices\n";
        delete [] x;
        delete [] y;
	Polygon::init();
        return -1;
    }

    // calculate the vertices then apply the transform
    double da = M_PI/nv*2.0;
    double ang = 0.0;
    int i;

    for (i = 0; i < nv; ++ i)
    {
        x[i] = xrad*cos(ang);
        y[i] = yrad*sin(ang);
        z[i] = 0.0;
        ang += da;
    }

    // transform the vertices
    t.transform(x, y, z, nv);

    valid = true;
    return 0;
}   // Calc


int Circle::SetNVertices(int nvert)
{
    if ((nvert < 3) || (nvert > 360))
    {
	ERRBLURB;
	cerr << "invalid number of vertices (" << nvert << ")\n";
	cerr << "\tValid values are 3 .. 360 only\n";
	return -1;
    }
    if (valid)
    {
	delete [] x;
	delete [] y;
	delete [] z;
	Polygon::init();
    }
    nv = nvert;
    return 0;
}

int Circle::StitchR(Rectangle& rect, Transform &t, VRMLMat &color, bool reuse_color,
            std::ofstream &fp, int tabs)
{
    return Polygon::Stitch(rect, t, color, reuse_color, fp, tabs);
}

int Circle::StitchC(Circle& circ, Transform &t, VRMLMat &color, bool reuse_color,
            std::ofstream &fp, int tabs)
{
    return Polygon::Stitch(circ, t, color, reuse_color, fp, tabs);
}
