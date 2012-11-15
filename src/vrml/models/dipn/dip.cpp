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
#include <iomanip>

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

DipParams::DipParams()
{
    // defaults are for DIP-24 with 0.3" row spacing and 0.1" pin-pin spacing
    A1 = 0.015;     // [BaseHeight] distance from board to bottom of case
    A2 = 0.115;     // [CaseDepth]
    L = 0.15;       // [ThruLength] pin length (narrow part)
    e = 0.1;        // [Pin Pitch]
    E = 0.3;        // [RowSpacing] pin row spacing (for example, 0.3 inches on typical DIP)
    E1 = 0.25;      // [CaseWidth]
    B1 = 0.06;      // [PinBreadth] widest part of the pin
    b = 0.022;      // [ThruWidth] width of narrow part of pin
    c = 0.014;      // [Thickness] pin material thickness

    NW = 0.06;          // [NotchWidth]
    ND = 0.012;         // [NotchDepth]
    NL = 0.07;          // [NotchLength] must be > NW/2
    MID = 0.022;        // [CaseMidHeight] depth of middle portion where pins attach
    DW = 0.005;         // [CaseAlw]
    S = 0.005;          // [CaseTaper] deviation of unbeveled top and bottom edges
    casebev = 0.005;    // [CaseBevel]
    pinbev = 0.002;     // Pin edge bevel

    scale = 10;

    return;
}

int Dip::SetPinColor(std::string filename)
{
    return pinmaterial.Load(filename);
}

int Dip::SetCaseColor(std::string filename)
{
    return casematerial.Load(filename);
}



Dip::Dip()
{
    pins = 0;
    haspin = NULL;
    return;
}

Dip::~Dip()
{
    if ((pins) && (haspin)) delete [] haspin;
    return;
}

int Dip::SetPins(int npins)
{
    if ((pins) && (haspin)) delete [] haspin;
    haspin = NULL;
    pins = 0;

    if ((npins < 4) || (npins % 2))
    {
        ERRBLURB;
        cerr << "npins (" << npins << ") must be >= 4 and a multiple of 2\n";
        return -1;
    }

    haspin = new (nothrow) bool [npins];
    if (haspin == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate memory for pin inclusion map\n";
        return -1;
    }
    int i;
    for (i = 0; i < npins; ++i) haspin[i] = true;

    pins = npins;
    return 0;
}

int Dip::SetPin(int pin, bool on)
{
    if (!pins)
    {
        ERRBLURB;
        cerr << "invoked without setting number of pins\n";
        return -1;
    }

    if ((pin < 1) || (pin > pins))
    {
        ERRBLURB;
        cerr << "out of range (pin = " << pin << "); must be 1 .. pins (" << pins << ")\n";
        return -1;
    }

    haspin[pin -1] = on;
    return 0;
}


int Dip::Build(std::string filename)
{
    double xoff, yoff, zoff;
    int pin, hpin;
    ofstream fp;
    int acc = 0;

    if (pins == 0)
    {
        ERRBLURB;
        cerr << "invoked without setting pins\n";
        return -1;
    }
    if ((pins < 4) || (pins % 2))
    {
        ERRBLURB;
        cerr << "pins < 4 or not a multiple of 2: " << pins << "\n";
        return -1;
    }

    DipCase iccase;
    DipPin icpin;

    const double tan5 = 0.0874886635259;
    if (params.DW < 0.0)
    {
        params.DW = 2.0*(params.c/3.0 + 0.5*params.B1);
    }
    if (params.MID < 0.0)
    {
        params.MID = 1.5*params.c;
    }
    if (params.S < 0.0)
    {
        params.S = (params.A2 - params.MID)*0.5*tan5;
    }

    // length of case, nominal Pins/2 + DW
    double D = pins*0.5*params.e + 0.5* params.DW;

    iccase.setCaseLength(D);
    iccase.setCaseWidth(params.E1);
    iccase.setBaseHeight(params.A1);
    iccase.setCaseDepth(params.A2);
    iccase.setCaseTaper(params.S);
    iccase.setCaseMidHeight(params.MID);
    iccase.setNotchLength(params.NL);
    iccase.setNotchDepth(params.ND);

    // derived pin parameters
    double h = params.A1 + 0.5*params.A2;
    double w = 0.5*(params.E - params.E1);
    acc += icpin.Calc(h, params.L, w, params.c, params.B1, params.b, params.pinbev);
    if (acc)
    {
        ERRBLURB;
        cerr << "problems calculating pin appearance\n";
        return -1;
    }

    xoff = -(pins/4.0 -0.5)*params.e;
    yoff = -params.E1*0.5;
    zoff = 0.0;

    Transform T, TC;
    Quat offset(0, xoff, yoff, zoff);
    Translation tr(offset);
    Rotation rot(0,0,0,1);
    T.setTranslation(tr);
    T.setRotation(rot);
    TC.setScale(params.scale);

    acc += iccase.calc();
    if (acc)
    {
        ERRBLURB;
        cerr << "problems calculating case appearance\n";
        return -1;
    }

    if (SetupVRML(filename, fp))
    {
        ERRBLURB;
        cerr << "could not open file '" << filename << "'\n";
        return -1;
    }

    casematerial.WriteMaterial(fp, 0, true);
    pinmaterial.WriteMaterial(fp, 0, true);

    // Case
    ostringstream partname;
    partname << "DIL" << pins << "_P" << setfill('0') << setw(5) << (int)(params.E*1000.0) << setfill(' ') << setw(0);

    acc += SetupXForm(partname.str(), fp, 0);

    acc += SetupShape(casematerial, true, fp, 2);
    acc += iccase.writeCoord(TC, fp, 4);
    acc += iccase.writeFacets(fp, 4);
    acc += CloseShape(fp, 2);
    if (acc)
    {
        ERRBLURB;
        cerr << "problems writing case data to file\n";
        return -1;
    }

    T.setScale(params.scale);
    // Pin1
    if (haspin[0]) acc += icpin.Build(T, pinmaterial, true, fp, 2);

    // Pin 2 .. (pins/2)
    hpin = pins/2;
    for (pin = 2; pin <= hpin; ++pin) {
        offset.x = offset.x + params.e;
        if (!(haspin[pin -1])) continue;
        tr.set(offset);
        T.setTranslation(tr);
        acc += icpin.Build(T, pinmaterial, true, fp, 2);
    }

    // Pin (pins/2 +1)..
    rot.set(M_PI, 0, 0, 1);
    T.setRotation(rot);
    offset.y = -offset.y;
    tr.set(offset);
    T.setTranslation(tr);
    for (pin = hpin+1; pin <= pins; ++pin) {
        if (haspin[pin -1]) acc += icpin.Build(T, pinmaterial, true, fp, 2);
        offset.x = offset.x - params.e;
        tr.set(offset);
        T.setTranslation(tr);
    }
    if (CloseXForm(fp)) return -1;

    return 0;
}



int Dip::SetParams(const DipParams &p)
{
    if (p.pinbev > p.c/3)
    {
        ERROUT("pin bevel (%f) is wider than 1/3 the pin material thickness (%f)\n", p.pinbev, p.c);
        return -1;
    }

    if (p.b < p.c)
    {
        ERROUT("pin thru width (%f) is less than the pin material thickness (%f)\n", p.b, p.c);
        return -1;
    }

    if (p.b >= p.B1)
    {
        ERROUT("pin thru width (%f) is less than or equal to the pin breadth (%f)\n", p.b, p.B1);
        return -1;
    }

    // XXX - validate parameters

    params = p;
    return 0;
}
