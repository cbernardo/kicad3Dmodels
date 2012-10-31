/*
 *      file: make19950.cpp
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
 *      This program generates 3 .. 40 pin 4UCon connectors, Drawing 19950
 *
 *
 *       ___o___   width = 2.5
 *       |     |
 *       |     |
 *       |     |   height of header = 8.5 (7.7 solid + 0.8 shoulder)
 *       |     |
 *       |_____|
 *       ||   ||
 *
 *       Shoulder = 0.8 high * 0.75 wide with a 0.15 radius
 *       Length   = 2.54 * npins + 0.25
 *       Pin-to-Pin length = 2.54 * (npins - 1)
 *       Pin (viewed from connector's long side): w = 0.64, d = 0.4
 */

#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "vdefs.h"
#include "keyvalparser.h"
#include "transform.h"
#include "vrmlmat.h"
#include "vcom.h"

#include "pin.h"
#include "rectangle.h"
#include "circle.h"
#include "funnel.h"
#include "hole.h"
#include "shoulder.h"


#include <iostream>
#include <fstream>
#include <string>


using namespace std;

int makeHdr(int np);

int main()
{
    int i;
    for (i = 3; i <= 40; ++i)
    {
        if (makeHdr(i))
        {
            cerr << "* errors creating " << i << "-pin part; terminating program\n";
            return -1;
        }
    }
    return 0;
}

int makeHdr(int np)
{
    Transform t0;
    PParams pinarg;
    Pin pin;

    if (np < 1)
    {
        ERRBLURB;
        cerr << "invalid number of pins (< 1)\n";
        return -1;
    }
    if (np > 100)
    {
        ERRBLURB;
        cerr << "invalid number of pins (> 100)\n";
        return -1;
    }

    pinarg.w = 0.64;
    pinarg.d = 0.4;
    pinarg.h = 0.8 + 10.5;  // shoulder height + pin length
    pinarg.tap = 0.8;       // 0.8mm tapered end
    pinarg.stw = 0.6;       // taper on w dimension is 60% of w
    pinarg.std = 1.0;       // no taper on d dimension
    pinarg.dbltap = false;
    t0.setTranslation(0, 0, -10.5); // shift the pin to the correct vertical location
    pin.Calc(pinarg, t0);

    // length
    double l = 2.54*np + 0.25;

    Shoulder bump;
    t0.setTranslation(0.0, 0.0, 0.0);   // center along X
    bump.Calc(l, 0.8, 0.75, 0.0, 0.3, t0);

    Funnel fun;
    t0.setTranslation(0.0, 0.0, 8.5);   // shift up to vertical location
    fun.Calc(1.8, 1.8, 0.9, 0.9, 0.5, 0.5, 4, t0);

    // bottom holes, leftmost, mid, rightmost
    Hole bh[3];
    t0.setTranslation(0.0, 0.0, 0.8);   // shift up to vertical location
    switch (np)
    {
    case 1:
        bh[0].Calc(2.79, 1.0, 0.64, 0.4, t0, true, 0, 0);
        break;
    case 2:
        bh[0].Calc(2.665, 1.0, 0.64, 0.4, t0, true, 0.0625, 0);
        bh[2].Calc(2.665, 1.0, 0.64, 0.4, t0, true, -0.0625, 0);
        break;
    default:
        bh[0].Calc(2.665, 1.0, 0.64, 0.4, t0, true, 0.0625, 0);
        bh[2].Calc(2.665, 1.0, 0.64, 0.4, t0, true, -0.0625, 0);
        bh[1].Calc(2.54, 1.0, 0.64, 0.4, t0, true, 0, 0);
        break;
    }

    // top holes, leftmost, mid, rightmost
    Hole th[3];
    t0.setTranslation(0.0, 0.0, 8.5);   // shift up to vertical location
    switch (np)
    {
    case 1:
        th[0].Calc(2.79, 2.5, 1.8, 1.8, t0, true, 0, 0);
        break;
    case 2:
        th[0].Calc(2.665, 2.5, 1.8, 1.8, t0, true, 0.0625, 0);
        th[2].Calc(2.665, 2.5, 1.8, 1.8, t0, true, -0.0625, 0);
        break;
    default:
        th[0].Calc(2.665, 2.5, 1.8, 1.8, t0, true, 0.0625, 0);
        th[2].Calc(2.665, 2.5, 1.8, 1.8, t0, true, -0.0625, 0);
        th[1].Calc(2.54, 2.5, 1.8, 1.8, t0, true, 0, 0);
        break;
    }

    Rectangle blk[2];   // casing
    t0.setTranslation(0.0, 0.0, 0.8);
    blk[0].Calc(l, 2.5, t0);
    t0.setTranslation(0.0, 0.0, 8.5);
    blk[1].Calc(l, 2.5, t0);

    Rectangle shd[2];   // shroud to  fix seams
    t0.setTranslation(0.0, 0.0, 0.9);
    shd[0].Calc(l-0.3, 2.2, t0);
    t0.setTranslation(0.0, 0.0, 8.4);
    shd[1].Calc(l-0.3, 2.2, t0);

    VRMLMat pcol, bcol; // pin and body color
    pcol.Load("/home/cirilo/projects/kicad/models/mcad/colors/gold.mat");
    bcol.Load("/home/cirilo/projects/kicad/models/mcad/colors/black.mat");

    ofstream output;
    ostringstream fname, bname;
    bname << "X4UCON_19950_" << np;
    fname << "4UCON_19950_" << np << ".wrl";

    int acc = 0;
    acc += SetupVRML(fname.str().c_str(), output);
    acc += SetupXForm(bname.str().c_str(), output, 0);

    double scale = 1.0/2.54;    // scale to KiCAD world
    t0.setScale(scale);

    // Add the shoulders
    t0.setTranslation(0, 1.25*scale, 0);
    acc += bump.Build(t0, bcol, false, output, 2);
    t0.setRotation(M_PI, 0, 0, 1);
    t0.setTranslation(0, -1.25*scale, 0);
    acc += bump.Build(t0, bcol, true, output, 2);

    // Add the casing and shroud
    t0.setRotation(0, 0, 0, 0);
    t0.setTranslation(0, 0, 0);
    acc += blk[0].Stitch(blk[1], true, t0, bcol, true, output, 2);
    acc += shd[0].Stitch(shd[1], true, t0, bcol, true, output, 2);

    // Add the top and bottom holes
    int i;
    double dx;  // displacement
    dx = -2.54*((np -1.0)/2.0);
    switch (np)
    {
    case 1:
        acc += bh[0].Build(false, t0, bcol, true, output, 2);
        break;
    case 2:
        t0.setTranslation((dx - 0.0625)*scale, 0, 0);
        acc += bh[0].Build(false, t0, bcol, true, output, 2);
        acc += th[0].Build(true, t0, bcol, true, output, 2);
        t0.setTranslation((-dx + 0.0625)*scale, 0, 0);
        acc += bh[2].Build(false, t0, bcol, true, output, 2);
        acc += th[2].Build(true, t0, bcol, true, output, 2);
        break;
    default:
        t0.setTranslation((dx - 0.0625)*scale, 0, 0);
        acc += bh[0].Build(false, t0, bcol, true, output, 2);
        acc += th[0].Build(true, t0, bcol, true, output, 2);
        t0.setTranslation((-dx + 0.0625)*scale, 0, 0);
        acc += bh[2].Build(false, t0, bcol, true, output, 2);
        acc += th[2].Build(true, t0, bcol, true, output, 2);
        for (i = 1; i < np -1; ++i)
        {
            t0.setTranslation((dx + 2.54*i)*scale, 0, 0);
            acc += bh[1].Build(false, t0, bcol, true, output, 2);
            acc += th[1].Build(true, t0, bcol, true, output, 2);
        }
        break;
    }

    // Add the pins and funnels
    bool reuse = false;
    for (i = 0; i < np; ++i)
    {
        t0.setTranslation((dx + 2.54*i)*scale, 0, 0);
        acc += pin.Build(true, false, t0, pcol, reuse, output, 2);
        reuse = true;
        acc += fun.Build(true, t0, bcol, true, pcol, true, output, 2);
    }

    CloseXForm(output, 2);
    output.close();

    if (acc)
    {
        ERRBLURB;
        cerr << "problems creating header " << fname.str().c_str() << "\n";
        return -1;
    }

    return 0;
}
