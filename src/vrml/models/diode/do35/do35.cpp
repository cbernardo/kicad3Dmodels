/*
 file: DO35.cpp

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

 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "vdefs.h"
#include "vcom.h"
#include "do35.h"
#include "transform.h"
#include "vrmlmat.h"
#include "circle.h"
#include "pin.h"

using namespace std;
using namespace kc3d;

namespace kc3ddiode
{

DO35::DO35()
{
    hascol = false;
    nwv = 16;
    ntv = 48;
    return;
}

DO35::~DO35()
{
    return;
}

int DO35::SetNVertices(int wire, int tube, int bend)
{
    if ((wire < 3) || (wire > 360))
    {
        ERRBLURB;
        cerr << "wire must have from 3 to 360 vertices\n";
        return -1;
    }
    if ((tube < 3) || (tube > 360))
    {
        ERRBLURB;
        cerr << "tube must have from 3 to 360 vertices\n";
        return -1;
    }
    if ((bend < 2) || (bend > 90))
    {
        ERRBLURB;
        cerr << "bend must have a value from 2 to 90\n";
        return -1;
    }
    nwv = wire;
    ntv = tube;
    nb = bend;
    return 0;
}

int DO35::SetColors(const std::string &wirecolor, const std::string &glasscolor,
        const std::string &cathodecolor, const std::string &tubecolor)
{
    int acc = 0;
    acc += wcol.Load(wirecolor);
    acc += gcol.Load(glasscolor);
    acc += kcol.Load(cathodecolor);
    acc += tcol.Load(tubecolor);
    if (acc)
    {
        hascol = false;
        ERRBLURB;
        cerr << "problems loading colors\n";
        return -1;
    }
    hascol = true;
    return 0;
}

int DO35::Build(const std::string &partname, double scale, bool horiz, bool vflip, double pitch, double lead)
{
    /*
     * Dimensions per DO-35 package specification:
     *      + D: 1.53 .. 2.28   (body diameter)
     *      + B: 3.05 .. 4.15   (body length)
     *      + C: 0.46 .. 0.56   (lead diameter)
     *      + A: 25.40 .. 38.10 (lead length - doesn't really matter)
     *
     * Other dimensions:
     *      + 0.9: edge taper (chamfer is 10% of body diameter)
     *      + 0.5: thickness of cathode band
     *      + 0.7: inner tube is 70% of outer body size
     *      + 0.3: 0.3mm is the thickness of the glass at the ends
     *      + vertical offsets:
     *          > horiz: ~D/2 + C == 1.7 mm to axis of body
     *          > vert: C == 0.56 to edge of body
     */

    Circle body[5]; // outer body (including cathode)
    Circle tube;    // inner tube
    Pin leads[2];

    // constraint: colors must be successfully loaded (hascol = true)
    if (!hascol)
    {
        ERRBLURB;
        cerr << "invoked without prior successful call to SetColors()\n";
        return -1;
    }

    // constraint: pitch must be >= 5.0 for horiz, >= 2.0 for vert
    if ((horiz) && (pitch < 5.0))
    {
        ERRBLURB;
        cerr << "pitch (" << pitch << ") is below minimum for horizontal orientation (5.0mm)\n";
        return -1;
    }
    else if ((!horiz) && (pitch < 2.0))
    {
        ERRBLURB;
        cerr << "pitch (" << pitch << ") is below minimum for vertical orientation (2.0mm)\n";
        return -1;
    }

    // constraint: vflip may only be true for vert orientation (otherwise ignored, but a warning is issued)
    if (horiz && vflip)
    {
        ERRBLURB;
        cerr << "WARNING: vflip is true while orientation is horizontal\n";
        vflip = false;
    }

    // constraint: lead must be >= 2.0
    if (lead < 2.0)
    {
        ERRBLURB;
        cerr << "lead length below top of PCB (" << lead << "must be >= 2.0 mm\n";
        return -1;
    }

    int i;
    for (i = 0; i < 5; ++i) body[i].SetNVertices(ntv);
    tube.SetNVertices(ntv);

    Transform t0, t1;
    Quat q0;
    double r, l0, l1;   // bend radius and lengths for lead
    PParams pp;
    pp.tap = -1.0;
    pp.w = 0.56;
    pp.d = 0.56;
    leads[0].SetShape(false);
    leads[1].SetShape(false);
    if (horiz)
    {
        // leads
        t0.setTranslation(0, 0, -lead);
        r = 0.84;               // 0.56 * 1.5
        l0 = 1.7 - r + lead;    // length of vertical section
        l1 = (pitch - 4.15 + 0.6)/2.0 - r;  // length of horizontal section
        pp.h = l0;
        pp.r = r;
        pp.nb = nb;
        pp.l = l1;
        pp.bend = M_PI/2.0;
        pp.ns = nwv;
        leads[0].Calc(pp, t0);
        t0.setTranslation(pitch, 0, -lead);
        t0.setRotation(M_PI, 0, 0, 1);
        leads[1].Calc(pp, t0);
        // glass body
        l0 = (pitch - 4.15)/2.0;
        t0.setTranslation(l0, 0, 1.7);
        t0.setRotation(M_PI/2.0, 0, 1, 0);
        l1 = 0.9*2.28;
        body[0].Calc(l1, l1, t0);
        l0 += 0.228;
        t0.setTranslation(l0, 0, 1.7);
        body[1].Calc(2.28, 2.28, t0);
        l0 += 0.772;    // 0.772 = 1mm cathode band - 0.228mm envelope edge
        t0.setTranslation(l0, 0, 1.7);
        body[2].Calc(2.28, 2.28, t0);
        l0 = (pitch - 4.15)/2.0 + 4.15 - 0.228;
        t0.setTranslation(l0, 0, 1.7);
        body[3].Calc(2.28, 2.28, t0);
        l0 += 0.228;
        t0.setTranslation(l0, 0, 1.7);
        body[4].Calc(l1, l1, t0);
        // inner tube
        l0 = (pitch - 4.15)/2.0 + 0.3;
        l1 = 1.596; // 2.28 * 0.7
        t0.setTranslation(l0, 0, 1.7);
        tube.Calc(l1, l1, t0);
    }
    else
    {
        // leads
        t0.setTranslation(0, 0, -lead);
        l0 = lead + 0.86;   // 0.86 = 0.56 + 0.3
        pp.h = l0;
        pp.r = -1;
        pp.nb = nb;
        pp.l = -1;
        pp.bend = 0.0;
        pp.ns = nwv;
        leads[0].Calc(pp, t0);
        t0.setTranslation(0, 0, 4.41);  // 4.41 = 0.56 + 4.15 - 0.3
        pp.h = 0.5;
        pp.r = pitch/2.0;
        pp.l = lead + 4.91; // 4.91 = 4.41 + 0.5
        pp.bend = M_PI;
        leads[1].Calc(pp, t0);
        // inner tube
        l1 = 1.596; // 2.28 * 0.7
        t0.setTranslation(0, 0, 0.86);  // 0.86 = 0.56 + 0.3
        tube.Calc(l1, l1, t0);
        // glass body
        t0.setTranslation(0, 0, 0.56);
        l1 = 0.9*2.28;
        body[0].Calc(l1, l1, t0);
        t0.setTranslation(0, 0, 0.788); // 0.788 = 0.56 + 0.228
        body[1].Calc(2.28, 2.28, t0);
        if (!vflip)
        {
            t0.setTranslation(0, 0, 1.56); // 1.56 = 0.56 + 1.0 cathode band
        }
        else
        {
            t0.setTranslation(0, 0, 3.71); // 3.71 = 0.56 + 4.15 - 1.0 cathode band
        }
        body[2].Calc(2.28, 2.28, t0);
        t0.setTranslation(0, 0, 4.482); // 4.21 = 0.56 + 4.15 - 0.228
        body[3].Calc(2.28, 2.28, t0);
        t0.setTranslation(0, 0, 4.71);  // 4.71 = 0.56 + 4.15
        body[4].Calc(l1, l1, t0);
    }

    int acc = 0;

    // open the file
    ofstream fp;
    ostringstream filename;
    filename << partname << ".wrl";
    acc += SetupVRML(filename.str(), fp);
    if (acc)
    {
        ERRBLURB;
        cerr << "could not open output file '" << filename.str().c_str() << "'\n";
        return -1;
    }
    acc += SetupXForm(partname, fp, 0);

    l0 = pitch/2.0;
    t0.setTranslation(-l0, 0, 0);
    t0.setRotation(0, 0, 0, 0);
    t0.setScale(scale);
    if (horiz)
    {
        // leads
        acc += leads[0].Build(true, false, t0, wcol, false, fp, 2);
        acc += leads[1].Build(true, false, t0, wcol, true, fp, 2);
        // inner tube
        t1.setTranslation(3.55, 0, 0);  // 3.55 = 4.15 - 2*0.3
        q0.set(0, (pitch - 4.15)/2.0 + 0.3, 0, 0);
        acc += tube.Extrude(true, true, true, q0, t1, t0, tcol, false, fp, 2);
        // glass
        acc += body[0].Paint(false, t0, kcol, false, fp, 2);
        acc += body[1].Paint(true, t0, kcol, false, fp, 2);
        acc += body[0].Stitch(true, body[1], t0, kcol, true, fp, 2);
        acc += body[1].Stitch(true, body[2], t0, kcol, true, fp, 2);
        acc += body[2].Stitch(true, body[3], t0, gcol, false, fp, 2);
        acc += body[3].Stitch(true, body[4], t0, gcol, true, fp, 2);
        acc += body[4].Paint(true, t0, gcol, true, fp, 2);
    }
    else
    {
        // leads
        acc += leads[0].Build(true, false, t0, wcol, false, fp, 2);
        acc += leads[1].Build(false, true, t0, wcol, true, fp, 2);
        // inner tube
        t1.setTranslation(0, 0, 3.55);  // 3.55 = 4.15 - 2*0.3
        q0.set(0, 0, 0, 0.86);             // 0.86 = 0.56 + 0.3
        acc += tube.Extrude(true, true, true, q0, t1, t0, tcol, false, fp, 2);
        // glass
        if (!vflip)
        {
            acc += body[0].Paint(false, t0, kcol, false, fp, 2);
            acc += body[1].Paint(true, t0, kcol, false, fp, 2);
            acc += body[0].Stitch(true, body[1], t0, kcol, true, fp, 2);
            acc += body[1].Stitch(true, body[2], t0, kcol, true, fp, 2);
            acc += body[2].Stitch(true, body[3], t0, gcol, false, fp, 2);
            acc += body[3].Stitch(true, body[4], t0, gcol, true, fp, 2);
            acc += body[4].Paint(true, t0, gcol, true, fp, 2);
        }
        else
        {
            acc += body[0].Paint(false, t0, gcol, false, fp, 2);
            acc += body[0].Stitch(true, body[1], t0, gcol, true, fp, 2);
            acc += body[1].Stitch(true, body[2], t0, gcol, true, fp, 2);
            acc += body[2].Stitch(true, body[3], t0, kcol, false, fp, 2);
            acc += body[3].Stitch(true, body[4], t0, kcol, true, fp, 2);
            acc += body[3].Paint(false, t0, kcol, false, fp, 2);
            acc += body[4].Paint(true, t0, kcol, true, fp, 2);
        }
    }

    acc += CloseXForm(fp, 0);
    fp.close();

    if (acc)
    {
        ERRBLURB;
        cerr << "problems writing diode data to file\n";
        return -1;
    }

    return 0;
}   // Build()

} /* namespace kc3d */
