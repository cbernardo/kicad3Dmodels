/*
 *      file: funnel.cpp
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
 *
 *      This class represents a rectangular or circular funnel which can
 *      be used to create recessed pins.
 *
 */

#include <iostream>
#include <fstream>
#include <new>

#include "vdefs.h"
#include "vcom.h"
#include "funnel.h"
#include "transform.h"
#include "circle.h"
#include "rectangle.h"
#include "vrmlmat.h"

using namespace std;
using namespace kc3d;

Funnel::Funnel()
{
    valid = false;
    square = true;
    npoly = 0;
    poly = NULL;
    return;
}

Funnel::Funnel(const Funnel &p)
{
    valid = p.valid;
    square = p.square;
    npoly = p.npoly;
    poly = NULL;

    if (!valid) npoly = 0;
    if (npoly == 0) return;

    poly = new (nothrow) Polygon *[npoly];
    if (poly == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate memory for polygon pointers\n";
        valid = false;
        npoly = 0;
        return;
    }

    int i, j;
    for (i = 0; i < npoly; ++i)
    {
        poly[i] = p.poly[i]->clone();
        if (!poly[i])
        {
            for (j = 0; j < i; ++j) delete poly[j];
            delete [] poly;
            poly = NULL;
            ERRBLURB;
            cerr << "could not allocate memory for polygon pointers\n";
            valid = false;
            npoly = 0;
            return;
        }
    }

    return;
}


Funnel::~Funnel()
{
    cleanup();
}


Funnel &Funnel::operator=(const Funnel &p)
{
    if (this == &p) return *this;

    if (valid) cleanup();

    valid = p.valid;
    square = p.square;
    npoly = p.npoly;
    poly = NULL;

    if (!valid) npoly = 0;
    if (npoly == 0) return *this;

    poly = new (nothrow) Polygon *[npoly];
    if (poly == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate memory for polygon pointers\n";
        valid = false;
        npoly = 0;
        return *this;
    }

    int i, j;
    for (i = 0; i < npoly; ++i)
    {
        poly[i] = p.poly[i]->clone();
        if (!poly[i])
        {
            for (j = 0; j < i; ++j) delete poly[j];
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

int Funnel::Calc(double w1, double d1, double w2, double d2,
        double h1, double h2, double h3, Transform &t, int ns)
{
    if (valid) cleanup();

    int np;
    bool has_h2 = false;
    if (h2 > 0.0)
    {
        has_h2 = true;
        np = 4;
    }
    else
    {
        np = 3;
    }

    // validate parameters
    if ((square) && (bev > 0.0))
    {
        if (bev*2.0 >= d2)
        {
            ERRBLURB;
            cerr << "invalid for d2 (<= bevel/2)\n";
            return -1;
        }
        if (bev*2.0 >= w2)
        {
            ERRBLURB;
            cerr << "invalid for w2 (<= bevel/2)\n";
            return -1;
        }
    }
    if (w1 <= 0.0)
    {
        ERRBLURB;
        cerr << "invalid value for w1 (<= 0)\n";
        return -1;
    }
    if (w2 <= 0.0)
    {
        ERRBLURB;
        cerr << "invalid value for w2 (<= 0)\n";
        return -1;
    }
    if (w1 <= w2)
    {
        ERRBLURB;
        cerr << "invalid value for w1 (<= w2)\n";
        return -1;
    }
    if (d1 <= 0.0)
    {
        ERRBLURB;
        cerr << "invalid value for d1 (<= 0)\n";
        return -1;
    }
    if (d2 <= 0.0)
    {
        ERRBLURB;
        cerr << "invalid value for d2 (<= 0)\n";
        return -1;
    }
    if (d1 <= d2)
    {
        ERRBLURB;
        cerr << "invalid value for d1 (<= d2)\n";
        return -1;
    }
    if (h1 < 0.0)
    {
        ERRBLURB;
        cerr << "invalid value for h1 (< 0)\n";
        return -1;
    }
    if (h3 <= 0.0)
    {
        ERRBLURB;
        cerr << "invalid value for h3 (<= 0)\n";
        return -1;
    }

    poly = new (nothrow) Polygon *[np];
    if (!poly)
    {
        ERRBLURB;
        cerr << "could not allocate memory for polygon pointers\n";
        return -1;
    }

    int i, j;
    if (square)
    {
        for (i = 0; i < np; ++i)
        {
            poly[i] = new (nothrow) Rectangle(-1.0);
            if (!poly[i])
            {
                ERRBLURB;
                cerr << "could not allocate memory for polygons\n";
                for (j = 0; j < i; ++j) delete poly[j];
                delete [] poly;
                poly = NULL;
                return -1;
            }
            static_cast <Rectangle *> (poly[i])->SetBevel(bev);
        }
    }
    else
    {
        int cs = ns;
        if ((cs < 3) || (cs > 360) || (cs % 4))
        {
            ERRBLURB;
            cerr << "invalid number of sides (" << ns << "); valid range is 4 .. 360 in multiples of 4\n";
            cerr << "\tUsing default 16\n";
            cs = 16;
        }
        for (i = 0; i < np; ++i)
        {
            poly[i] = new (nothrow) Circle(cs);
            if (!poly[i])
            {
                ERRBLURB;
                cerr << "could not allocate memory for polygons\n";
                for (j = 0; j < i; ++j) delete poly[j];
                delete [] poly;
                poly = NULL;
                return -1;
            }
        }
    }

    int acc = 0;
    int idx = 0;
    Transform t0;
    acc += poly[idx++]->Calc(w1, d1, t0);
    t0.setTranslation(0, 0, -h1);
    acc += poly[idx++]->Calc(w2, d2, t0);
    if (has_h2)
    {
        t0.setTranslation(0, 0, -(h1 + h2));
        acc += poly[idx++]->Calc(w2, d2, t0);
        t0.setTranslation(0, 0, -(h1 + h2 + h3));
    }
    else
    {
        t0.setTranslation(0, 0, -(h1 + h3));
    }
    acc += poly[idx++]->Calc(w2, d2, t0);

    npoly = np;
    valid = true;
    if (acc)
    {
        ERRBLURB;
        cerr << "problems encountered while creatign funnel\n";
        cleanup();
        return -1;
    }

    // apply the transform
    for (i = 0; i < np; ++ i) poly[i]->Xform(t);

    return 0;
}

int Funnel::Build(bool cap, Transform &t, VRMLMat &flutecolor, bool reuse_flute,
        VRMLMat &stemcolor, bool reuse_stem, std::ofstream &fp, int tabs)
{
    if (!valid)
    {
        ERRBLURB;
        cerr << "invoked with no prior successful call to Calc()\n";
        return -1;
    }

    if (npoly < 2)
    {
        ERRBLURB;
        cerr << "BUG: invalid number of polygons (min. 2): " << npoly << "\n";
        return -1;
    }

    bool has_h2 = false;
    if (npoly == 4) has_h2 = true;

    int acc = 0;
    int idx = 0;
    bool reuse = reuse_flute;
    acc += poly[idx]->Stitch(true, *poly[idx +1], t, flutecolor, reuse, fp, tabs);
    ++idx;
    if (has_h2)
    {
        acc += poly[idx]->Stitch(true, *poly[idx +1], t, flutecolor, true, fp, tabs);
        ++idx;
    }
    // stem
    reuse = reuse_stem;
    if (!flutecolor.GetName().compare(stemcolor.GetName()))
    {
        reuse = true;
    }
    acc += poly[idx]->Stitch(true, *poly[idx +1], t, stemcolor, reuse, fp, tabs);
    ++idx;
    if (cap)
    {
        acc += poly[idx]->Paint(true, t, stemcolor, false, fp, tabs);
    }
    if (acc)
    {
        ERRBLURB;
        cerr << "problems encountered creating funnel\n";
        return -1;
    }

    return 0;
}   // Build()



void Funnel::cleanup(void)
{
    int i;
    if (valid)
    {
        for (i = 0; i < npoly; ++i) delete poly[i];
        delete [] poly;
        poly = NULL;
    }
    valid = false;
    npoly = 0;
}

void Funnel::SetShape(bool square, double bev)
{
    if (valid) cleanup();
    Funnel::square = square;
    Funnel::bev = bev;
    return;
}
