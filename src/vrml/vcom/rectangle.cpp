/*
 *      file: rectangle.cpp
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
#include "transform.h"
#include "vrmlmat.h"
#include "polygon.h"
#include "rectangle.h"

using namespace std;
using namespace kc3d;

#define MIN_LEN (0.000001)

Rectangle::Rectangle()
{
    bev = -1.0;
    seg = 1;
    nv = 0;
    Polygon::init();
    return;
}

Rectangle::Rectangle(double bevel)
{
    nv = 0;
    Polygon::init();
    bev = bevel;
    return;
}

// Clone the object
Polygon *Rectangle::clone(void) const
{
    return new (nothrow) Rectangle(*this);
}

Rectangle::~Rectangle()
{
    if (x) delete [] x;
    if (y) delete [] y;
    if (z) delete [] z;
    Polygon::init();
    return;
}


Rectangle::Rectangle(const Rectangle &p)
{
    valid = p.valid;
    nv = p.nv;

    x = NULL;
    y = NULL;
    z = NULL;

    if (!valid) nv = 0;
    if (nv == 0) return;

    x = new (nothrow) double [nv];
    if (x == NULL)
    {
        valid = false;
        nv = 0;
        return;
    }

    y = new (nothrow) double [nv];
    if (y == NULL)
    {
        delete [] x;
        valid = false;
        nv = 0;
        return;
    }

    z = new (nothrow) double [nv];
    if (z == NULL)
    {
        delete [] x;
        delete [] y;
        valid = false;
        nv = 0;
        return;
    }

    int i;
    for (i = 0; i < nv; ++i)
    {
        x[i] = p.x[i];
        y[i] = p.y[i];
        z[i] = p.z[i];
    }
    
    bev = p.bev;

    return;
}


Rectangle & Rectangle::operator=(const Rectangle &p)
{
    if (this == &p) return *this;

    if (nv)
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

    if (!valid) nv = 0;
    if (nv == 0) return *this;

    x = new (nothrow) double [nv];
    if (x == NULL)
    {
        valid = false;
        nv = 0;
        return *this;
    }

    y = new (nothrow) double [nv];
    if (y == NULL)
    {
        delete [] x;
        valid = false;
        nv = 0;
        return *this;
    }

    z = new (nothrow) double [nv];
    if (z == NULL)
    {
        delete [] x;
        delete [] y;
        valid = false;
        nv = 0;
        return *this;
    }

    int i;
    for (i = 0; i < nv; ++i)
    {
        x[i] = p.x[i];
        y[i] = p.y[i];
        z[i] = p.z[i];
    }

    bev = p.bev;
    
    return *this;
}

int Rectangle::Calc(double xl, double yl, Transform &t)
{
    if (valid)
    {
        if (x) delete [] x;
        if (y) delete [] y;
        if (z) delete [] z;
        Polygon::init();
	nv = 0;
    }

    int np = 4;
    if (bev > 0.0) np += 4*seg;

    if ((bev >= xl/2.0) || (bev > yl/2.0))
    {
        ERRBLURB;
        cerr << "invalid bevel (equals or exceeds side/2)\n";
        return -1;
    }

    if (xl < MIN_LEN)
    {
        ERRBLURB;
        cerr << "invalid X length (" << xl << "). Minimum is " << MIN_LEN << "\n";
        return -1;
    }

    if (yl < MIN_LEN)
    {
        ERRBLURB;
        cerr << "invalid Y length (" << xl << "). Minimum is " << MIN_LEN << "\n";
        return -1;
    }

    x = new (nothrow) double[np];
    if (x == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for vertices\n";
        Polygon::init();
        nv = 0;
        return -1;
    }
    y = new (nothrow) double[np];
    if (y == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for vertices\n";
        delete [] x;
        Polygon::init();
        nv = 0;
        return -1;
    }
    z = new (nothrow) double[np];
    if (z == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate points for vertices\n";
        delete [] x;
        delete [] y;
        Polygon::init();
        nv = 0;
        return -1;
    }

    // calculate the vertices then apply the transform
    int i;
    for (i = 0; i < np; ++i) z[i] = 0.0;
    double apx, apy;    // anchor points for calculation of bevel/arc
    double ang, dang;   // angle, increment in angle
    switch (np)
    {
    case 4:
        x[0] = xl/2.0;
        x[1] = x[0];
        x[2] = -x[0];
        x[3] = x[2];
        y[0] = -yl/2.0;
        y[1] = - y[0];
        y[2] = y[1];
        y[3] = y[0];
        break;
    default:
        apx = xl/2.0 - bev;
        apy = -yl/2.0 + bev;
        dang = 0.5*M_PI/seg;
        ang = -0.5*M_PI;
        for (i = 0; i <= seg; ++i)
        {
            x[i] = apx + bev*cos(ang);
            y[i] = apy + bev*sin(ang);
            x[2*(seg +1) - i -1] = x[i];
            y[2*(seg +1) - i -1] = -y[i];
            x[2*(seg +1) + i] = -x[i];
            y[2*(seg +1) + i] = -y[i];
            x[4*(seg +1) - i -1] = -x[i];
            y[4*(seg +1) - i -1] = y[i];
            ang += dang;
        }
        break;
    }

    // transform the vertices
    t.transform(x, y, z, np);

    valid = true;
    nv = np;
    return 0;
}   // Calc


int Rectangle::SetBevel(double bevel, int segments)
{
    bev = bevel;
    if (segments <= 0) segments = 1;
    if (segments > 90)
    {
        ERRBLURB;
        cerr << "invalid number of segments; must be <= 90\n";
        return -1;
    }
    seg = segments;
    return 0;
}
