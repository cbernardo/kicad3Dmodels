/*
 file: dip_pin.cpp

 Copyright 2012, Cirilo Bernardo (cjh.bernardo@gmail.com)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>

 Notes:
 These classes were created to support the
 programmatic creation of VRML models for
 use with KiCad.

 TODO:
 * check constraints
 */

#include <fstream>
#include <iostream>
#include <cmath>
#include <iomanip>

#include "vdefs.h"
#include "dip_pin.h"
#include "transform.h"
#include "vrmlmat.h"
#include "pin.h"

using namespace std;
using namespace kc3d;

// the radius coefficient; R = RC*c
#define RC (1.0)

namespace kc3ddip {

DipPin::DipPin()
{
    valid = false;
    return;
}

// Calculate the vertices
int DipPin::Calc(double h, double l, double w, double t, double pb, double tb, double bev)
{
    valid = false;
    double r = RC*t;

    if (l <= 0.0)
    {
        ERRBLURB;
        cerr << "length of pin's thru portion (l) must be > 0\n";
        return -1;
    }

    if (t <= 0.0)
    {
        ERRBLURB;
        cerr << "pin's material thickness (t) must be > 0\n";
        return -1;
    }

    if (pb <= 0.0)
    {
        ERRBLURB;
        cerr << "pin's breadth (pb) must be > 0\n";
        return -1;
    }

    if ((tb <= 0.0) || (tb >= pb))
    {
        ERRBLURB;
        cerr << "pin's thru breadth (tb) must be > 0 and < pb\n";
        return -1;
    }

    // check if the bevel interferes with features
    if ((bev > t/4.0) || (bev > tb/4.0))
    {
        ERRBLURB;
        cerr << "pin's bevel (bev) must be < t/4 and < tb/4\n";
        return -1;
    }

    if (w < r)
    {
        ERRBLURB;
        cerr << "pin extent (w = " << setprecision(8) << w <<
                ") is less than mandated radius (r = " << RC << "*t = "
                << r << ")\n";
        return -1;
    }

    const double tan40 = 0.839099631177;
    double taper = (pb - tb)/2.0*tan40;
    if (h <= (r + taper))
    {
        ERRBLURB;
        cerr << "pin's attachment height (h) must be > (" << RC << "*t + (pb - pt)/2*tan(40))\n";
        return -1;
    }

    Transform t0;
    t0.setTranslation(0, -w, -l);
    t0.setRotation(M_PI*0.5, 0 ,0 ,1);

    PParams pp;
    pp.bev = bev;
    pp.d = tb;
    pp.w = t;
    pp.dbltap = false;
    pp.h = l;
    pp.r = -0.1;
    pp.l = -0.1;
    pp.tap = bev;
    if (bev > 0)
    {
        pp.stw = 1.0 - 2.0*bev/t;
        pp.std = 1.0 - 2.0*bev/tb;
    }
    int acc = 0;
    acc += pin[0].Calc(pp, t0);

    t0.setTranslation(0, -w, 0);
    pp.d = pb;
    pp.h = h - r;
    pp.tap = taper;
    pp.r = r;
    pp.bend = 0.5*M_PI;
    pp.nb = 3;
    pp.l = w - r;
    pp.stw = 1.0;
    pp.std = tb/pb;
    acc += pin[1].Calc(pp, t0);
    if (acc)
    {
        ERRBLURB;
        cerr << "problems calculating pin vertices\n";
        return -1;
    }

    valid = true;
    return 0;
}

// Write the shape information to a file
int DipPin::Build(kc3d::Transform &t, kc3d::VRMLMat &color, bool reuse, std::ofstream &file, int tabs)
{
    if (!valid)
    {
        ERRBLURB;
        cerr << "invoked with no prior successful call to Calc()\n";
        return -1;
    }

    int acc = 0;
    acc += pin[0].Build(true, false, t, color, reuse, file, tabs);
    acc += pin[1].Build(false, false, t, color, true, file, tabs);
    if (acc)
    {
        ERRBLURB;
        cerr << "problems writing pin data to file\n";
        return -1;
    }
    return 0;
}

}
