/*
 *      file: resistor.cpp
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
#include <cstring>
#include <cctype>
#include <cmath>
#include <new>
#include <iomanip>
#include <sstream>

#include "vdefs.h"
#include "vcom.h"
#include "circle.h"
#include "transform.h"
#include "vrmlmat.h"
#include "resistor.h"
#include "pin.h"

using namespace std;
using namespace kc3d;
using namespace kc3dresistor;


RParams::RParams()
{
    // set defaults for LR0204 series 0.25W MFR, millimeters
    scale = 1.0/2.54;// allows for internal units in mm and VRML scale of 1unit = 0.1 inch
    shift = 0;
    L   = 3.5;      // length of body
    D   = 2.0;      // diameter of body
    d   = 0.45;     // diameter of wire
    p   = 10.16;    // lead pitch (0.4 inch)
    wl  = 4.0;      // length below top of PCB caters for 1.6 .. 3.0mm boards
    horiz = true;   // orientation; true if horizontal
    endshape = 'B'; // 'B'ulge end finish
    bcap = false;   // no metallic cap on Bulge end style
    wsides = 16;    // number of sides to the wire
    bsides = 6;     // number of sides in a 90-deg bend
    rsides = 48;    // number of sides on the resistor
    return;
}


int Resistor::makeHzLead(std::ofstream &fp)
{
    // XXX - TODO: refactor using class Pin

    // Notes:
    //  bottom point: (0, 0, -params.wl)
    //  top point:    (0, 0, (D/2 - d))
    //  end of bend:  (1.5d, 0, (D/2 + 0.2))
    //  end of hz:    (((p - L)/2 - 0.5d), 0, (D/2 + d/2))
    //  end of cap:   ((p - L)/2, 0, (D/2 + d/2))

    // vertical lead
    Circle lvert[2];
    lvert[0].SetNVertices(params.wsides);
    lvert[1].SetNVertices(params.wsides);
    // bend; first polygon = last polygon of vertical lead
    int nb = params.bsides + 1;
    Circle *bend = new (nothrow) Circle[nb];
    if (bend == NULL)
    {
        ERRBLURB;
        cerr << "could not allocate memory for calculations\n";
        return -1;
    }
    int i;
    for (i = 0; i < nb; ++i) bend[i].SetNVertices(params.wsides);
    // horizontal lead; first polygon = last polygon of bend
    Circle lhz[2];
    lhz[0].SetNVertices(params.wsides);
    lhz[1].SetNVertices(params.wsides);
    Circle cap[2];
    cap[0].SetNVertices(params.wsides);
    cap[1].SetNVertices(params.wsides);

    // vertical lead
    Transform tv;
    tv.setTranslation(0, 0, -params.wl);
    double rw = params.d;   // wire diameter (formerly radius)
    lvert[0].Calc(rw, rw, tv);
    tv.setTranslation(0, 0, params.D/2 - params.d);
    lvert[1].Calc(rw, rw, tv);

    // bend
    double ba = M_PI/params.bsides/2.0; // incremental bend angle
    double br = 1.5*params.d;       // bend radius
    double px, pz;  // x and z positions of the wire centers on the bend
    double ph = params.D/2.0 - params.d;  // z offset
    bend[0] = lvert[1];
    double ang;
    for (i = 1; i < nb; ++i)
    {
        ang = i*ba;
        px = br*(1.0 - cos(ang));
        pz = br*sin(ang) + ph;
        tv.setRotation(ang, 0, 1, 0);
        tv.setTranslation(px, 0, pz);
        bend[i].Calc(rw, rw, tv);
    }

    // horizontal lead
    lhz[0] = bend[nb -1];
    pz = (params.D + params.d)/2.0;
    if ((params.endshape == 'B')&&(params.bcap))
    {
        // leave space for the metallic cap
        px = (params.p - params.L)/2.0 - params.d/3.0;
    }
    else
    {
        // butt up to case
        px = (params.p - params.L)/2.0;
    }
    tv.setRotation(M_PI/2.0, 0, 1, 0);
    tv.setTranslation(px, 0, pz);
    lhz[1].Calc(rw, rw, tv);

    // metallic cap (only when endshape option is 'B'
    if ((params.endshape == 'B')&&(params.bcap))
    {
        cap[0].Calc(2.0*params.d, 2.0*params.d, tv);
        px = (params.p - params.L)/2.0;
        tv.setTranslation(px, 0, pz);
        cap[1].Calc(2.0*params.d, 2.0*params.d, tv);
    }

    // paint the wire end and stitch the vertical part
    Transform t0;
    t0.setScale(params.scale);
    // keep in mind that the translation is the last transform and is unaffected by scale
    t0.setTranslation(params.shift, 0, 0);
    lvert[0].Paint(false, t0, params.colors[13], false, fp, 2);
    lvert[0].Stitch(true, lvert[1], t0, params.colors[13], true, fp, 2);
    // stitch the bend
    for(i = 0; i < nb-1; ++i)
    {
        bend[i].Stitch(true, bend[i+1], t0, params.colors[13], true, fp, 2);
    }
    // stitch the horizontal part
    lhz[0].Stitch(true, lhz[1], t0, params.colors[13], true, fp, 2);
    if ((params.endshape == 'B')&&(params.bcap))
    {
        // paint and stitch the cap
        cap[0].Paint(false, t0, params.colors[13], true, fp, 2);
        cap[0].Stitch(true, cap[1], t0, params.colors[13], true, fp, 2);
    }

    // repeat the paint and stitch operations but transform the positions
    t0.setRotation(M_PI, 0, 0, 1);
    t0.setTranslation(params.p + params.shift, 0, 0);
    // vertical part
    lvert[0].Paint(false, t0, params.colors[13], true, fp, 2);
    lvert[0].Stitch(true, lvert[1], t0, params.colors[13], true, fp, 2);
    // stitch the bend
    for(i = 0; i < nb-1; ++i)
    {
        bend[i].Stitch(true, bend[i+1], t0, params.colors[13], true, fp, 2);
    }
    // stitch the horizontal part
    int val = lhz[0].Stitch(true, lhz[1], t0, params.colors[13], true, fp, 2);
    if ((params.endshape == 'B')&&(params.bcap))
    {
        // paint and stitch the cap
        cap[0].Paint(false, t0, params.colors[13], true, fp, 2);
        val = cap[0].Stitch(true, cap[1], t0, params.colors[13], true, fp, 2);
    }
    return val;
}



int Resistor::makeVtLead(std::ofstream &fp)
{
    Transform t0;
    PParams lp;
    Pin lead[2];
    Pin cap[2];

    // select round wires
    lead[0].SetShape(false);
    lead[1].SetShape(false);
    cap[0].SetShape(false);
    cap[1].SetShape(false);

    bool has_mcap = false;
    double th0 = 2.0*params.d - params.d/3.0;               // height of cap0
    double th1 = 2.0*params.d + params.L;                   // height of cap1
    double th2 = 2.0*params.d + params.L + params.d/3.0;    // height of lead1

    t0.setTranslation(0, 0, -params.wl);
    if ((params.endshape == 'B') && (params.bcap))
    {
        lp.h = params.wl + 2.0*params.d - params.d/3.0;
        has_mcap = true;
    }
    else
    {
        lp.h = params.wl + 2*params.d;
    }
    lp.w    = params.d; // wire thickness (X axis)
    lp.d    = params.d; // wire thickness (Y axis)
    lp.tap  = -1.0;     // no taper
    lp.r    = -1.0;     // no bend
    lp.l    = -1.0;     // no second part
    lp.ns   = params.wsides;

    // starting lead
    lead[0].Calc(lp, t0);

    // caps (if necessary)
    if (has_mcap)
    {
        lp.h = params.d/3.0;
        lp.w = params.d*2.0;
        lp.d = lp.w;
        t0.setTranslation(0, 0, th0);
        cap[0].Calc(lp, t0);
        t0.setTranslation(0, 0, th1);
        cap[1].Calc(lp, t0);
    }

    // end lead
    lp.h = 2.0*params.d;
    if (has_mcap)
    {
        t0.setTranslation(0, 0, th2);
        lp.l = th2 + params.wl + lp.h;
    }
    else
    {
        t0.setTranslation(0, 0, th1);
        lp.l = th1 + 4.0 + lp.h;
    }
    lp.r = params.p/2.0;
    lp.bend = M_PI;
    lp.nb = 2*params.bsides;
    lp.w = params.d;
    lp.d = params.d;
    lead[1].Calc(lp, t0);

    t0.setTranslation(params.shift, 0, 0);
    t0.setScale(params.scale);

    int acc = 0;
    acc += lead[0].Build(true, false, t0, params.colors[13], false, fp, 2);
    if (has_mcap)
    {
        acc += cap[0].Build(true, false, t0, params.colors[13], true, fp, 2);
        acc += cap[1].Build(false, true, t0, params.colors[13], true, fp, 2);
    }
    acc += lead[1].Build(false, true, t0, params.colors[13], true, fp, 2);

    if (acc)
    {
        ERRBLURB;
        cerr << "problems writing vertical resistor lead\n";
        return -1;
    }

    return 0;
}



int Resistor::makeBody(std::ofstream &fp, const std::string &bands)
{
    // Notes:
    //  Hz start of body:   ((p - L)/2, 0, (D/2 + 0.2))
    //  Vt start of body:   (0, 0, 2*d)
    //
    //  The polygons are calculated with the end of the body at (0, 0, 0).
    //  The assembly is shifted into an appropriate position upon stitching.

    int nend;   // number of polygons at one end only
    int ntot;   // total number of polygons
    int nb = bands.size() +1;

    if (nb < 1)
    {
        ERRBLURB;
        cerr << "invalid number of bands (" << nb << "); must be >4 characters\n";
        return -1;
    }

    double q;   // width of the end shapes = q*params.D
    switch (params.endshape)
    {
    case 'C':
        nend = 3;
        q = 0.25;
        break;
    case 'R':
        nend = params.bsides +1;
        q = 0.5;
        break;
    default:    // assume 'B'
        nend = params.bsides + 1;
        q = 0.25;
        break;
    }
    ntot = 2*nend + 2*(nb -1);

    Circle *body = new (nothrow) Circle[ntot];
    if (!body)
    {
        ERRBLURB;
        cerr << "could not allocate memory for polygons\n";
        return -1;
    }
    int i;
    for (i = 0; i < ntot; ++i) body[i].SetNVertices(params.rsides);

    Transform t0;   // transform reference
    Transform t1;   // variable transform
    Transform tx;   // temporary transform
    Translation off0;   // reference offset along resistor axis
    Translation off1;   // variable offset to adjust t1
    // set up the transform based on Hz or Vt orientation
    if (params.horiz)
    {
        off0.set(1, 0, 0);
        t0.setRotation(M_PI/2.0, 0, 1, 0);
    }
    else
    {
        off0.set(0, 0, 1);
    }
    t0.setTranslation(off1);
    t1 = t0;

    // create the end shapes
    double rad;
    switch (params.endshape)
    {
    case 'C':
        // first annulus is D/20 wide and 0.9D diameter
        rad = params.D*0.9/2.0;
        body[0].Calc(rad*2.0, rad*2.0, t1);
        off1 = off0*params.D/20.0;
        t1.setTranslation(off1);
        rad = params.D/2.0;
        body[1].Calc(rad*2.0, rad*2.0, t1);
        // second annulus is at 0.5D
        off1 = off0*0.25*params.D;
        t1.setTranslation(off1);
        body[2].Calc(rad*2.0, rad*2.0, t1);
        // calculate the other end
        body[ntot -1] = body[0];
        body[ntot -2] = body[1];
        body[ntot -3] = body[2];
        tx.setTranslation(params.L*off0);
        body[ntot -1].Xform(tx);
        tx.setTranslation((params.L - params.D/10.0)*off0);
        body[ntot -2].Xform(tx);
        tx.setTranslation((params.L - params.D*.5)*off0);
        body[ntot -3].Xform(tx);
        break;
    case 'R':
        do
        {
            double ao = q*params.D;  // anchor at wide end of cap
            double ao2 = ao*ao;
            double k = (params.D - params.d)/(2.0*ao);  // magic ratio
            double r;   // radius of circle
            double x;   // x position
            double xx;  // x distance from center of sphere
            for (i = 0; i < params.bsides +1; ++i)
            {
                x = i*ao/params.bsides;
                xx = ao - x;
                xx = ao2 - xx*xx;
                if (xx < 0.0) xx = 0.0;
                r = k*sqrt(xx) + params.d/2.0;
                t1.setTranslation(off0*x);
                body[i].Calc(r*2.0, r*2.0, t1);
                tx.setTranslation(off0*(params.L - 2*x));
                body[ntot -1 - i] = body[i];
                body[ntot -1 - i].Xform(tx);
            }
        } while (0);
        break;
    default:    // assume 'B'
        do
        {
            double ao = 0.25*params.D;  // anchor at wide end of cap
            double ao2 = ao*ao;
            // double rmin = 0.25*params.D;
            double rmin = params.d/2.0;
            if (params.bcap) rmin = 1.05*params.d;
            double k = (0.5*params.D - rmin)/(0.25*params.D); // magic ratio
            double r;   // radius of circle
            double x;   // x position
            double xx;  // x distance from center of sphere
            for (i = 0; i < params.bsides +1; ++i)
            {
                x = i*ao/params.bsides;
                xx = ao - x;
                xx = ao2 - xx*xx;
                if (xx <= 0.0) xx = 0.0;
                r = sqrt(xx)*k + rmin;
                t1.setTranslation(off0*x);
                body[i].Calc(r*2.0, r*2.0, t1);
                tx.setTranslation(off0*(params.L - 2*x));
                body[ntot -1 - i] = body[i];
                body[ntot -1 - i].Xform(tx);
            }
        } while (0);
        break;
    }

    if (params.endshape == 'B')
    {
        double ao = 0.25*params.D;  // sphere center at near end of cap
        double ao2 = ao*ao;
        double ao3 = params.L - 0.25*params.D;  // sphere center at far end of cap
        double x = params.D*0.43;
        double xx;
        double dx = (params.L - 0.5*params.D)/(nb -1);
        double sx = 0.25*params.D;
        double r;
        int j = nend -1;
        for (i = 1; i < nb; ++i)
        {
            sx += dx;
            if ((i < nend) && (sx <= 0.375*params.D))
            {
                xx = sx - ao;
                xx = ao2 - xx*xx;
                if (xx < 0.0) xx = 0;
                r = sqrt(xx) + 0.25*params.D;
            }
            else if ((i >= nend - nb) && (sx >= params.L - 0.375*params.D))
            {
                x = ao3 - sx;
                xx = ao2 - x*x;
                if (xx < 0.0) xx = 0;
                r = sqrt(xx) + 0.25*params.D;
            } else
            {
                r = x;
            }
            t1.setTranslation(off0*sx);
            body[i+j].Calc(r*2.0, r*2.0, t1);
        }
    }
    else
    {
        double dx = (params.L - 2.0*q*params.D)/(nb -1);
        double sx = q*params.D;
        double r = params.D/2.0;
        int j = nend -1;
        for (i = 1; i < nb; ++i)
        {
            sx += dx;
            t1.setTranslation(off0*sx);
            body[i+j].Calc(r*2.0, r*2.0, t1);
        }
    }

    // write out data
    tx.setScale(params.scale);
    // the translation depends on orientation
    if (params.horiz)
    {
        tx.setTranslation((params.p - params.L)/2.0 + params.shift,
                0, (params.D + params.d)/2.0);
    }
    else
    {
        tx.setTranslation( params.shift, 0, 2.0*params.d);
    }
    body[0].Paint(false, tx, params.colors[12], false, fp, 2);
    for (i = 0; i < nend -1; ++i)
    {
        body[i].Stitch(true, body[i+1], tx, params.colors[12], true, fp, 2);
    }
    int j = nend -1;
    int idx;
    bool used[14];
    for (i = 0; i < 14; ++i) used[i] = false;
    used[12] = true;    // body color is already used
    for (i = 0; i < nb -1; ++i)
    {
        idx = getColorIdx(i, bands);
        body[i+j].Stitch(true, body[i+j+1], tx, params.colors[idx], used[idx], fp, 2);
        if (!used[idx]) used[idx] = true;
    }
    for (i = ntot - nend; i < ntot -1; ++i)
    {
        body[i].Stitch(true, body[i+1], tx, params.colors[12], true, fp, 2);
    }
    int val = body[ntot -1].Paint(true, tx, params.colors[12], false, fp, 2);
    delete [] body;
    return val;
}



int Resistor::Create(RParams &rp, const std::string &bands, const std::string &filename)
{
    int i;
    bool fail = false;
    std::ofstream fp;

    if (filename.empty())
    {
        ERRBLURB;
        cerr << "empty filename\n";
        return -1;
    }

    /* validate the parameters */
    // ensure that the colors are set up
    for (i = 0; i < 14; ++i)
    {
        if (rp.colors[i].GetName().empty())
        {
            ERRBLURB;
            cerr << "color #" << i << " does not have a valid name\n";
            fail = true;
        }
    }
    // wire depth must be >0 (since we dont know the units we can't do any more checking)
    if (rp.wl < 1e-9)
    {
        ERRBLURB;
        cerr << "wire depth below top of PCB must be > 0\n";
        fail = true;
    }
    // lead spacing must be:
    //      horiz: >= L + 4*d
    //      vert:  >= D + 1.5*d
    if (rp.horiz)
    {
        if (rp.p < (rp.L + 4.0*rp.d))
        {
            ERRBLURB;
            cerr << "[horiz] lead pitch too small (" << setprecision(4) << "); must be >= L+4*d (" <<
                    rp.L + 4.0*rp.d << ")\n";
            fail = true;
        }
    }
    else
    {
        if (rp.p < (rp.D + 1.5*rp.d))
        {
            ERRBLURB;
            cerr << "[vert] lead pitch too small (" << setprecision(4) << "); must be >= D+1.5*d (" <<
                    rp.D + 1.5*rp.d << ")\n";
            fail = true;
        }
    }
    // ensure reasonable values for number of vertices in the circular sections
    if ((rp.wsides < 8)||(rp.wsides > 360))
    {
        ERRBLURB;
        cerr << "invalid number of sides on wire (" << rp.wsides << "); valid range is 8..360\n";
        fail = true;
    }
    if ((rp.bsides < 4)||(rp.bsides > 24))
    {
        ERRBLURB;
        cerr << "invalid number of sides on wire bend (" << rp.bsides << "); valid range is 4..24\n";
        fail = true;
    }
    if ((rp.rsides < 3)||(rp.rsides > 360))
    {
        ERRBLURB;
        cerr << "invalid number of sides on resistor (" << rp.rsides << "); valid range is 3..360\n";
        fail = true;
    }
    if ((rp.endshape != 'C') && (rp.endshape != 'R') && (rp.endshape != 'B'))
    {
        ERRBLURB;
        cerr << "invalid end shape ('" << rp.endshape << "'); valid values are 'C', 'R', and 'B'\n";
        fail = true;
    }
    if (validateBands(bands))
    {
        ERRBLURB;
        cerr << "invalid band code\n";
        fail = true;
    }
    if (fail) return -1;


    params = rp;
    ostringstream fname;
    fname << filename << params.endshape;
    if (!params.pwrsuf.empty()) fname << "_" << params.pwrsuf;
    if (!params.spcsuf.empty()) fname << "_" << params.spcsuf;
    string modname(fname.str());
    fname << ".wrl";
    if (SetupVRML(fname.str(), fp))
    {
        ERRBLURB;
        cerr << "could not open file: '" << filename << "'\n";
        return -1;
    }
    // write all data to file and check the results at the end
    int acc = 0;
    acc += SetupXForm(modname, fp, 0);
    if (params.horiz)
    {
        acc += makeHzLead(fp);
        acc += makeBody(fp, bands);
    }
    else
    {
        acc += makeVtLead(fp);
        acc += makeBody(fp, bands);
    }
    acc += CloseXForm(fp, 0);
    fp.close();

    if (acc)
    {
        ERRBLURB;
        cerr << "problems writing to file '" << filename << "'\n";
        return -1;
    }

    return 0;
}   // Create


int Resistor::validateBands(const std::string &bands)
{
    int i;
    int fail = 0;

    if (bands.empty())
    {
        ERRBLURB;
        cerr << "the band code must have at least one character\n";
        return -1;
    }
    const char *cp = bands.c_str();
    int len = strlen(cp);

    // The rules have been relaxed; any position may be one of 0..9, X, S, G
    for (i = 0; i < len; ++i)
    {
        if ((!isdigit(cp[i])) && (cp[i] != 'X') &&
                (cp[i] != 'S') && (cp[i] != 'G'))
        {
            if (fail)
            {
                cerr << "\t";
            }
            else
            {
                ERRBLURB;
            }
            cerr << "invalid code at position " << i+1 << " in '" << cp << "'\n";
            fail = -1;
        }
    }

    return fail;
}

int Resistor::getColorIdx(int i, const std::string &bands)
{

    unsigned char t = bands[i];
    int idx = 0;
    if ((t >= '0')&&(t <= '9'))
    {
        idx = t - 48;
    }
    else if (t == 'G')
    {
        idx = 10;
    }
    else if (t == 'S')
    {
        idx = 11;
    }
    else
    {
        idx = 12;
    }
    return idx;
}
