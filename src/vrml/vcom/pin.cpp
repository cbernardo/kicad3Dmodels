/*
 *      file: pin.cpp
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
#include <new>

#include "vdefs.h"
#include "pin.h"
#include "transform.h"
#include "vrmlmat.h"
#include "polygon.h"
#include "polyrect.h"

using namespace std;

PParams::PParams()
{
    // defaults are in mm for a typical blind pin
    w = 0.6;
    d = 0.6;
    tap = 0.5;
    stw = 0.8;
    std = 0.8;
    dbltap = false;
    h = 8.0;
    r = -1.0;       // no bend
    nb = 5;         // 5 segments to a 90-deg bend
    l = -1.0;       // no horizontal section
    bev = -1.0;     // default to no bevel
    bend = M_PI/2.0;
    ns = 24;
    return;
}

Pin::Pin()
{
    valid = false;
    poly = NULL;
    nr = 0;
    square = true;
    return;
}

Pin::~Pin()
{
    if (poly)
    {
        delete [] poly;
        poly = NULL;
        nr = 0;
    }
    return;
}

// Calculate the intermediate polygons
int Pin::Calc(const PParams &pp, Transform &t)
{
    bool has_h = false; // true if there is a vertical section
    bool has_b = false; // true if there is a bend
    bool has_l = false; // true if there is a horizontal section
    bool has_t = false; // true if there is a taper

    if (valid)
    {
        valid = false;
        if (nr)
        {
            nr = 0;
            delete [] poly;
        }
    }
    // validate parameters
    if (pp.h > 0.0)
    {
        has_h = true;
        if (pp.tap > 0)
        {
            if (pp.tap > pp.h)
            {
                ERRBLURB;
                cerr << "taper > height\n";
                return -1;
            }
            if ((pp.dbltap) && (pp.r <= 0.0) && (pp.tap*2.0 >= pp.h))
            {
                ERRBLURB;
                cerr << "double taper >= height\n";
                return -1;
            }
            has_t = true;
        }
    }

    if (pp.l > 0.0)
    {
        has_l = true;
        if ((pp.dbltap) && (pp.tap > 0))
        {
            has_t = true;
            if (pp.tap > pp.l)
            {
                ERRBLURB;
                cerr << "taper > length\n";
                return -1;
            }
        }
    }

    if (pp.r > 0)
    {
        has_b = true;
        if ((pp.nb < 3) || (pp.nb > 90))
        {
            ERRBLURB;
            cerr << "invalid number of segments in a bend (" << pp.nb << "); valid range is 3 .. 90\n";
            return -1;
        }
        if (pp.r < pp.w)
        {
            ERRBLURB;
            cerr << "bend radius < width\n";
            return -1;
        }
        if ((pp.bend <= 0.0) || (pp.bend > M_PI))
        {
            ERRBLURB;
            cerr << "invalid number bend angle (" << pp.bend << "); valid range is >0.0 and <=M_PI\n";
            return -1;
        }
    }

    if (pp.bev > 0.0)
    {
        if ((pp.bev*2.0 >= pp.w)||(pp.bev*2.0 >= pp.d))
        {
            ERRBLURB;
            cerr << "invalid bevel specification (must be < 1/2 of either side)\n";
            return -1;
        }
        if ((pp.tap > 0.0) &&
                ((pp.bev*2.0 >= pp.w*pp.stw)||(pp.bev*2.0 >= pp.d*pp.std)))
        {
            ERRBLURB;
            cerr << "bad bevel specification; bevel will be too large on tapered segment\n";
            return -1;
        }
    }

    if ((!has_b) && (!has_h) && (!has_l))
    {
        ERRBLURB;
        cerr << "invalid specification; pin has no features\n";
        return -1;
    }

    if ((has_l) && (has_h) && (!has_b))
    {
        ERRBLURB;
        cerr << "invalid specification; vertical and horizontal bars but no bend\n";
        return -1;
    }

    // calculate the number of polygons needed
    int np = 1; // this represents the final polygon
    if (has_h)
    {
        ++np;
        if (has_t) ++np;
        if ((pp.dbltap) && (pp.r <= 0.0)) ++np;
    }
    if (has_b) np += pp.nb;
    if (has_l)
    {
        ++np;
        if ((has_t) && (pp.dbltap)) ++np;
    }

    if (square)
    {
        poly = new (nothrow) PolyRect[np];
    }
    else
    {
        if ((pp.ns < 3) || (pp.ns > 360))
        {
            ERRBLURB;
            cerr << "invalid number of sides for a polygon (" << pp.ns << ")\n";
            cerr << "\tValid values are 3 .. 360\n";
            return -1;
        }
        poly = new (nothrow) Polygon[np];
    }
    if (!poly)
    {
        ERRBLURB;
        cerr << "could not allocate memory for polygons\n";
        return -1;
    }

    // calculate the polygons
    pin = pp;
    int i, idx, acc;
    double px, pz;
    idx = 0;
    acc = 0;
    px = 0.0;
    pz = 0.0;
    Transform t0;

    if (has_h)
    {
        if (has_t)
        {
            if (square)
                acc += static_cast <PolyRect &> (poly[idx++]).Calc(pin.w*pin.stw, pin.d*pin.std, t0, pin.bev);
            else
                acc += poly[idx++].Calc(pin.ns, pin.w*pin.stw, pin.d*pin.std, t0);
            pz = pin.tap;
            t0.setTranslation(0, 0, pz);
        }
        if (square)
            acc += static_cast <PolyRect &> (poly[idx++]).Calc(pin.w, pin.d, t0, pin.bev);
        else
            acc += poly[idx++].Calc(pin.ns, pin.w, pin.d, t0);
        pz = pin.h;
        t0.setTranslation(0, 0, pz);
    }

    if (has_b)
    {
        double ba = pin.bend/pin.nb; // incremental bend angle
        double tx, tz;  // x and z positions of the wire centers on the bend
        double ang;

        if (square)
            acc += static_cast <PolyRect &> (poly[idx++]).Calc(pin.w, pin.d, t0, pin.bev);
        else
            acc += poly[idx++].Calc(pin.ns, pin.w, pin.d, t0);

        for (i = 1; i < pin.nb; ++i)
        {
            ang = i*ba;
            tx = pin.r*(1.0 - cos(ang));
            tz = pin.r*sin(ang) + pz;
            t0.setRotation(ang, 0, 1, 0);
            t0.setTranslation(tx, 0, tz);
            if (square)
                acc += static_cast <PolyRect &> (poly[idx++]).Calc(pin.w, pin.d, t0, pin.bev);
            else
                acc += poly[idx++].Calc(pin.ns, pin.w, pin.d, t0);
        }
        px = pin.r*(1-cos(pin.bend));
        pz = pz + pin.r*sin(pin.bend);
        t0.setTranslation(px, 0, pz);
        t0.setRotation(pin.bend, 0, 1, 0);
    }

    // Calculate an appropriate transform for the case
    // where only a horizontal section was specified.
    // Using the object in this way is not a recommended
    // means for producing a horizontal pin since the
    // one end will not be tapered under any circumstance
    if ((!has_h) && (!has_b))
    {
        px = 0;
        pz = 0;
        t0.setTranslation(0,0,0);
        t0.setRotation(M_PI/2.0, 0, 1, 0);
    }


    if (has_l)
    {
        if (square)
            acc += static_cast <PolyRect &> (poly[idx++]).Calc(pin.w, pin.d, t0, pin.bev);
        else
            acc += poly[idx++].Calc(pin.ns, pin.w, pin.d, t0);

        double cb = M_PI/2.0 - pin.bend;  // complement to bend
        if ((has_t) && (pin.dbltap))
        {
            if (pin.r < 0)
            {
                // special case of a horizontal feature only
                t0.setTranslation(pin.l - pin.tap, 0, pin.h);
                if (square)
                    acc += static_cast <PolyRect &> (poly[idx++]).Calc(pin.w, pin.d, t0, pin.bev);
                else
                    acc += poly[idx++].Calc(pin.ns, pin.w, pin.d, t0);
                t0.setTranslation(pin.l, 0, pin.h);
                if (square)
                    acc += static_cast <PolyRect &> (poly[idx++]).Calc(pin.w*pin.stw, pin.d*pin.std, t0, pin.bev);
                else
                    acc += poly[idx++].Calc(pin.ns, pin.w*pin.stw, pin.d*pin.std, t0);
            }
            else
            {
                t0.setTranslation(px + (pin.l - pin.tap)*cos(cb), 0,
                        pz + (pin.l - pin.tap)*sin(cb));
                if (square)
                    acc += static_cast <PolyRect &> (poly[idx++]).Calc(pin.w, pin.d, t0, pin.bev);
                else
                    acc += poly[idx++].Calc(pin.ns, pin.w, pin.d, t0);
                t0.setTranslation(px + pin.l*cos(cb), 0,
                        pz + pin.l*sin(cb));
                if (square)
                    acc += static_cast <PolyRect &> (poly[idx++]).Calc(pin.w*pin.stw, pin.d*pin.std, t0, pin.bev);
                else
                    acc += poly[idx++].Calc(pin.ns, pin.w*pin.stw, pin.d*pin.std, t0);
            }
        }
        else
        {
            t0.setTranslation(px + pin.l*cos(cb), 0,
                    pz + pin.l*sin(cb));
            if (square)
                acc += static_cast <PolyRect &> (poly[idx++]).Calc(pin.w, pin.d, t0, pin.bev);
            else
                acc += poly[idx++].Calc(pin.ns, pin.w, pin.d, t0);
        }
    }
    else
    {
        // add the final polygon to a pin with no horizontal wire
        if ((!has_b) && (has_t) && (pin.dbltap))
        {
            t0.setTranslation(0, 0, pin.h - pin.tap);
            if (square)
                acc += static_cast <PolyRect &> (poly[idx++]).Calc(pin.w, pin.d, t0, pin.bev);
            else
                acc += poly[idx++].Calc(pin.ns, pin.w, pin.d, t0);
            t0.setTranslation(0, 0, pin.h);
            if (square)
                acc += static_cast <PolyRect &> (poly[idx++]).Calc(pin.w*pin.stw, pin.d*pin.std, t0, pin.bev);
            else
                acc += poly[idx++].Calc(pin.ns, pin.w*pin.stw, pin.d*pin.std, t0);
        }
        else
        {
            if (square)
                acc += static_cast <PolyRect &> (poly[idx++]).Calc(pin.w, pin.d, t0, pin.bev);
            else
                acc += poly[idx++].Calc(pin.ns, pin.w, pin.d, t0);
        }
    }

    if (acc < 0)
    {
        ERRBLURB;
        cerr << "errors encountered while building a square pin\n";
        delete [] poly;
        return -1;
    }

    valid = true;
    nr = np;
    return 0;
}   // Calc


// Write out the pin shape information
int Pin::Build(bool cap0, bool cap1, Transform &t, VRMLMat &color, bool reuse_color,
        std::ofstream &fp, int tabs)
{
    if (!valid)
    {
        ERRBLURB;
        cerr << "invoked with no prior successful call to Calc()\n";
        return -1;
    }
    if (nr < 2)
    {
        ERRBLURB;
        cerr << "BUG: invalid number of polygons (min. 2): " << nr << "\n";
        return -1;
    }
    int vl = 0;
    bool reuse = reuse_color;

    if (cap0)
    {
        vl += poly[0].Paint(false, t, color, reuse, fp, tabs);
        reuse = true;
    }
    if (cap1)
    {
        vl += poly[nr -1].Paint(true, t, color, reuse, fp, tabs);
        reuse = true;
    }
    vl += poly[0].Stitch(poly[1], true, t, color, reuse, fp, tabs);

    int i;
    for (i = 1; i < nr-1; ++i)
    {
        vl += poly[i].Stitch(poly[i+1], true, t, color, true, fp, tabs);
    }

    if (vl)
    {
        ERRBLURB;
        cerr << "problems encountered while rendering part\n";
        return -1;
    }
    return 0;
}


void Pin::SetShape(bool square)
{
    if (valid)
    {
        if (nr)
        {
            delete [] poly;
            poly = NULL;
            nr = 0;
            valid = false;
        }
    }
    Pin::square = square;
    return;
}
