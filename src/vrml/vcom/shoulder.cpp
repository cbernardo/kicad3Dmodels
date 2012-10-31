/*
 *      file: shoulder.cpp
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
 *      This class represents a shoulder such as the one found on the
 *      bottom of a SIL header which creates a small gap between the
 *      component and the board.
 *
 */

#include <iostream>
#include <fstream>
#include <cmath>

#include "shoulder.h"
#include "vdefs.h"
#include "vcom.h"
#include "transform.h"

using namespace std;

Shoulder::Shoulder()
{
    valid = false;
    np = 0;
    return;
}

Shoulder::~Shoulder()
{
    valid = false;
    return;
}

int Shoulder::Calc(double l, double h, double w, double t, double r, Transform &tx)
{
    valid = false;

    // validate the parameters
    if (l <= 0.0)
    {
        ERRBLURB;
        cerr << "invalid length (<= 0)\n";
        return -1;
    }
    if (h <= 0.0)
    {
        ERRBLURB;
        cerr << "invalid height (<= 0)\n";
        return -1;
    }
    if (w <= 0.0)
    {
        ERRBLURB;
        cerr << "invalid width (<= 0)\n";
        return -1;
    }

    bool has_t = false;
    bool has_r = false;

    if (t > 0.0)
    {
        has_t = true;
        if (t > 0.4*M_PI)
        {
            ERRBLURB;
            cerr << "invalid taper (> 0.4*M_PI)\n";
            return -1;
        }
        if (l <= 2.0*h*tan(t))
        {
            ERRBLURB;
            cerr << "invalid taper; angle is too steep or length is too small\n";
            return -1;
        }
    }

    np = 4;
    if (r > 0.0)
    {
        has_r = true;
        np = 7;
        if (r >= w)
        {
            ERRBLURB;
            cerr << "invalid radius (>= width)\n";
            return -1;
        }
        if (r >= h)
        {
            ERRBLURB;
            cerr << "invalid radius (>= height)\n";
            return -1;
        }
    }

    int idx = 0;
    double l2 = l/2.0;

    // Point 0
    p0[0][idx] = -l2;
    p0[1][idx] = 0.0;
    p0[2][idx] = h;
    ++idx;

    // Point 1
    p0[0][idx] = -l2;
    p0[1][idx] = 0.0;
    p0[2][idx] = 0.0;
    if (has_t) p0[0][idx] += h*tan(t);
    ++idx;

    int i;
    if (has_r)
    {
        double da = 0.5*M_PI/3.0;
        double ang;
        for (i = 0; i < 4; ++i)
        {
            ang = i*da;
            p0[0][idx] = -l2;
            p0[1][idx] = -w + r*(1 - sin(ang));
            p0[2][idx] = r*(1 - cos(ang));
            if (has_t) p0[0][idx] += (h - p0[2][idx])*tan(t);
            ++idx;
        }
    }
    else
    {
        p0[0][idx] = -l2;
        p0[1][idx] = -w;
        p0[2][idx] = 0.0;
        if (has_t) p0[0][idx] += h*tan(t);
        ++idx;
    }

    // Last point
    p0[0][idx] = -l2;
    p0[1][idx] = -w;
    p0[2][idx] = h;

    // mirror the part
    for (idx = 0; idx < np; ++ idx)
    {
        p1[0][idx] = -p0[0][idx];
        p1[1][idx] = p0[1][idx];
        p1[2][idx] = p0[2][idx];
    }

    tx.transform(p0[0], p0[1], p0[2], np);
    tx.transform(p1[0], p1[1], p1[2], np);

    valid = true;
    return 0;
}

int Shoulder::Build(Transform &t, VRMLMat &color, bool reuse,
        std::ofstream &fp, int tabs)
{
    if (!valid)
    {
        ERRBLURB;
        cerr << "invoked with no prior successful call to Calc()\n";
        return -1;
    }

    double tp0[3][14];

    int i;
    for (i = 0; i < np; ++i)
    {
        tp0[0][i] = p0[0][i];
        tp0[1][i] = p0[1][i];
        tp0[2][i] = p0[2][i];
        tp0[0][i+np] = p1[0][i];
        tp0[1][i+np] = p1[1][i];
        tp0[2][i+np] = p1[2][i];
    }

    t.transform(tp0[0], tp0[1], tp0[2], np*2);

    // CAP0
    // set up VRML Shape
    SetupShape(color, reuse, fp, tabs);
    // enumerate vertices
    WriteCoord(tp0[0], tp0[1], tp0[2], np, fp, tabs +1);
    // enumerate facets
    SetupCoordIndex(fp, tabs +1);
    string fmt((tabs + 1)*4, ' ');
    fp << fmt << "    ";
    for (i = 0; i < np; ++i)
    {
        fp << i << ",";
    }
    fp << "0,-1\n";
    CloseCoordIndex(fp, tabs +1);
    CloseShape(fp, tabs);

    // CAP1
    // set up VRML Shape
    SetupShape(color, true, fp, tabs);
    // enumerate vertices
    WriteCoord(&tp0[0][np], &tp0[1][np], &tp0[2][np], np, fp, tabs +1);
    // enumerate facets
    SetupCoordIndex(fp, tabs +1);
    fp << fmt << "    ";
    for (i = np-1; i > 0; --i)
    {
        fp << i << ",";
    }
    fp << "0," << np-1 << ",-1\n";
    CloseCoordIndex(fp, tabs +1);
    CloseShape(fp, tabs);

    // Body
    // set up VRML Shape
    SetupShape(color, true, fp, tabs);
    // enumerate vertices
    WriteCoord(tp0[0], tp0[1], tp0[2], np*2, fp, tabs +1);
    // enumerate facets
    SetupCoordIndex(fp, tabs +1);
    fp << fmt << "    ";
    for (i = 0; i < np -2; ++i)
    {
        fp << i+1 << "," << i << "," <<
                i + np << "," << i + np +1 << ",-1,";
        if (!((i + 1) % 18))
            fp << "\n" << fmt << "    ";
    }
    fp << np -1 << "," << np -2 << "," <<
            np + np -2 << "," << np + np -1 << ",-1\n";
    CloseCoordIndex(fp, tabs +1);

    return CloseShape(fp, tabs);
}
