/*
  file: dip.cpp

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

    TODO:
  Notes:
    These classes were created to support the
    programmatic creation of VRML models for
    use with KiCad.

    ** This is very bare and there is a lot of hard-coding
        which was done to rush the generation of a set of models.

    ** Calc() should accept a list of pins to be rendered;
        this would provide some flexibility in creating odd DIP
        packages such as a magnetic reed switch which may have a
        long package but only 4 pins.
 */

#include <cstdio>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include "vdefs.h"
#include "vcom.h"
#include "transform.h"
#include "dip.h"
#include "dip_case.h"
#include "dip_pin.h"

using namespace std;
using namespace kc3d;
using namespace kc3ddip;

#define ERROUT(...) do {\
    fprintf(stderr, "%s:%d: %s(): ", __FILE__, __LINE__, __FUNCTION__);\
    fprintf(stderr, ##__VA_ARGS__); } while (0)

dipparams::dipparams()
{
    // defaults are for DIP-24 with 0.3" row spacing and 0.1" pin-pin spacing
    metric = false;
    scale = 10.0;   // World Scale; set to '10' for KiCAD representation (0.1inch = 1VRML unit)
    npins = 24;

    pinbev = 0.002; // Pin edge bevel
    B = 0.022;      // [ThruWidth] width of narrow part of pin
    B1 = 0.06;      // [PinBreadth] widest part of the pin
    B2 = 0.015;     // [TaperHeight] height of the pin's tapered portion
    C1 = 0.01;      // [DogEar] length of top bent (sloped) portion)
    C2 = 0.0025;    // [DogEarOffset] offset of lower bent (sloped) portion)
    C = 0.014;      // [Thickness] pin material thickness
    E = 0.3;        // [RowSpacing] pin row spacing (for example, 0.3 inches on typical DIP)
    L = 0.15;       // [ThruLength] pin length (narrow part)
    P = 0.1;        // [PinPitch]

    casebev = 0.005;    // [CaseBevel]
    A1 = 0.015;         // [BaseHeight] distance from board to bottom of case
    A2 = 0.115;         // [CaseDepth]
    E1 = 0.25;          // [CaseWidth]
    NW = 0.06;          // [NotchWidth]
    ND = 0.012;         // [NotchDepth]
    NL = 0.07;          // [NotchLength] must be > NW/2
    MID = 0.022;        // [CaseMidHeight] depth of middle portion where pins attach
    D = 1.25;           // [CaseLength]
    S = 0.005;          // [CaseTaper] deviation of unbeveled top and bottom edges
    DW = 0.005;         // [CaseAlw]

    return;
}




dip::dip()
{
    return;
}

int dip::Calc(int pins, std::string filename)
{
    /**
        TODO: at the moment this is tailored to
        only one class of DIP; it should be modified
        to take more information from the user about
        case and pin dimensions and material appearance

        TODO: enforce constraints (example: pins >= 4, pins%2 = 0)

        TODO: put in error checking on I/O (modify to throw/catch)
    */

    double xoff, yoff, zoff;
    int pin, hpin;
    ofstream fp;

    if (pins == 0) pins = params.npins;
    if ((pins < 4) || (pins % 2))
    {
        ERRBLURB;
        cerr << "pins < 4 or not a multiple of 2: " << pins << "\n";
        return -1;
    }

    dipcase iccase;
    dippin icpin;

    params.D = pins/2*params.P + params.DW;

    iccase.setMetric(params.metric);
    iccase.setCaseLength(params.D);
    iccase.setCaseWidth(params.E1);
    iccase.setBaseHeight(params.A1);
    iccase.setCaseDepth(params.A2);
    iccase.setCaseTaper(params.S);
    iccase.setCaseMidHeight(params.MID);
    iccase.setNotchLength(params.NL);
    iccase.setNotchDepth(params.ND);

    icpin.setMetric(params.metric);
    icpin.setBaseHeight(params.A1);
    icpin.setCaseDepth(params.A2);
    icpin.setThruWidth(params.B);
    icpin.setPinBreadth(params.B1);
    icpin.setDogEar(params.C1);
    icpin.setDogEarOffset(params.C2);
    icpin.setThickness(params.C);
    icpin.setCaseWidth(params.E1);
    icpin.setThruLength(params.L);
    icpin.setRowSpacing(params.E);

    /* note: offsets depend on the world scale */
    xoff = -(pins/4.0 -0.5)*params.P*params.scale;
    yoff = -params.E1*0.5*params.scale;
    zoff = 0.0;

    Transform T;
    Quat offset(0, xoff, yoff, zoff);
    Translation tr(offset);
    Rotation rot(0,0,0,1);
    T.setTranslation(tr);
    T.setRotation(rot);

    iccase.calc();
    icpin.calc();

    if (SetupVRML(filename, fp))
    {
        ERRBLURB;
        cerr << "could not open file '" << filename << "'\n";
        return -1;
    }

    // Case
    ostringstream partname;
    if (params.metric)
    {
        partname << "DIL" << pins << "_m" << icpin.getRowSpacing()*100;
    }
    else
    {
        partname << "DIL" << pins << "_i" << icpin.getRowSpacing()*1000;
    }

    SetupXForm(partname.str(), fp, 0);

    SetupShape(params.casematerial, false, fp, 2);
    iccase.writeCoord(fp, 4);
    iccase.writeFacets(fp, 4);
    CloseShape(fp, 2);

    // Pin1 (defines the pin material)
    SetupShape(params.pinmaterial, false, fp, 2);
    icpin.writeCoord(fp, 4, &T);
    icpin.writeFacets(fp, 4);
    CloseShape(fp, 2);

    // Pin 2 .. (pins/2)
    hpin = pins/2;
    for (pin = 2; pin <= hpin; ++pin) {
        offset.x = offset.x + 1.0;
        tr.set(offset);
        T.setTranslation(tr);
        SetupShape(params.pinmaterial, true, fp, 2);
        icpin.writeCoord(fp, 4, &T);
        icpin.writeFacets(fp, 4);
        CloseShape(fp, 2);
    }

    // Pin (pins/2 +1)..
    rot.set(M_PI, 0, 0, 1);
    T.setRotation(rot);
    offset.y = -offset.y;
    tr.set(offset);
    T.setTranslation(tr);
    for (pin = hpin+1; pin <= pins; ++pin) {
        SetupShape(params.pinmaterial, true, fp, 2);
        icpin.writeCoord(fp, 4, &T);
        icpin.writeFacets(fp, 4);
        CloseShape(fp, 2);
        offset.x = offset.x - 1.0;
        tr.set(offset);
        T.setTranslation(tr);
    }
    if (CloseXForm(fp)) return -1;

    return 0;
}



int dip::SetParams(const dipparams &p)
{
    params = p;
    // XXX - to implement - validate settings
    return -1;

    if (p.scale == 0)
    {
        ERROUT("scale factor is zero\n");
        return -1;
    }

    if (p.pinbev > p.C/3)
    {
        ERROUT("pin bevel (%f) is wider than 1/3 the pin material thickness (%f)\n", p.pinbev, p.C);
        return -1;
    }

    if (p.B < p.C)
    {
        ERROUT("pin thru width (%f) is less than the pin material thickness (%f)\n", p.B, p.C);
        return -1;
    }

    if (p.B >= p.B1)
    {
        ERROUT("pin thru width (%f) is less than or equal to the pin breadth (%f)\n", p.B, p.B1);
        return -1;
    }

    // XXX - RowSpacing must be greater than CaseWidth + 2*DogEar - PinThickness

    // XXX - DogEarOffset must be < (DogEar - PinThickness/2)


/*

    pinbev = 0.002; // Pin edge bevel
    B = 0.018;      // [ThruWidth] width of narrow part of pin
    B1 = 0.06;      // [PinBreadth] widest part of the pin
    B2 = 0.015;     // [TaperHeight] height of the pin's tapered portion
    C1 = 0.01;      // [DogEar] length of top bent (sloped) portion)
    C2 = 0.0025;    // [DogEarOffset] offset of lower bent (sloped) portion)
    C = 0.01;       // [Thickness] pin material thickness
    E = 0.3;        // [RowSpacing] pin row spacing (for example, 0.3 inches on typical DIP)
    L = 0.13;       // [ThruLength] pin length (narrow part)

    casebev = 0.005;    // [CaseBevel]
    A1 = 0.015;         // [BaseHeight] distance from board to bottom of case
    A2 = 0.13;          // [CaseDepth]
    E1 = 0.26;          // [CaseWidth]
    NW = 0.06;          // [NotchWidth]
    ND = 0.02;          // [NotchDepth]
    NL = 0.08;          // [NotchLength] must be > NW/2
    MID = 0.02;         // [CaseMidHeight] depth of middle portion where pins attach
    D = 1.25;           // [CaseLength]
    S = 0.01;           // [CaseTaper] deviation of unbeveled top and bottom edges
*/
}
