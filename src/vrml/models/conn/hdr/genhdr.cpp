/*
 *      file: genhdr.cpp
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

/// TODO:
/// 1. Support rectangular pins on bottom half of female headers
/// 2. Support round bottom pins with rectangular sockets on female headers
/// 3. Support +/- dimensions at the connector edges (break the enforced square shape rule)
/// NOTE: This requires similar support in the HdrCase class

#include <iostream>
#include <string>
#include <fstream>

#include "vrmlmat.h"
#include "transform.h"
#include "vdefs.h"
#include "vcom.h"
#include "hdrbase.h"
#include "circle.h"
#include "funnel.h"
#include "pin.h"
#include "genhdr.h"

using namespace std;
using namespace kc3d;

namespace kc3dconn {

Genhdr::Genhdr()
{
    hasColors = false;
    hasBparams = false;
    hasPparams = false;
    return;
}



int Genhdr::Build(Transform &t, std::string part, std::ofstream &fp, int tabs)
{
    if (part.empty())
    {
        ERRBLURB;
        cerr << "invalid part name\n";
        return -1;
    }

    if (!hasColors)
    {
        ERRBLURB;
        cerr << "color scheme not set\n";
        return -1;
    }
    if (!hasBparams)
    {
        ERRBLURB;
        cerr << "case parameters not set\n";
        return -1;
    }
    if (!hasPparams)
    {
        ERRBLURB;
        cerr << "pin parameters not set\n";
        return -1;
    }

    int val = 0;
    val += SetupXForm(part, fp, tabs);

    val += makeCase(t, fp, tabs+2);
    if (val)
    {
        ERRBLURB;
        cerr << "could not write case facets to file\n";
        return -1;
    }

    val += makePins(t, fp, tabs+2);
    if (val)
    {
        ERRBLURB;
        cerr << "could not write pin information to file\n";
        return -1;
    }

    if ((!male) && (!squaretop)) val += makeShrouds(t, fp, tabs+2);
    if (val)
    {
        ERRBLURB;
        cerr << "could not write shroud data to file\n";
        return -1;
    }

    if (!male) val += makeFunnels(t, fp, tabs+2);
    if (val)
    {
        ERRBLURB;
        cerr << "could not write funnel data to file\n";
        return -1;
    }

    val += CloseXForm(fp, tabs);
    if (val)
    {
        ERRBLURB;
        cerr << "problems creating generic header\n";
        return -1;
    }

    return 0;
}



int Genhdr::SetColors(std::string bcolor, std::string pcolor, std::string fcolor, std::string scolor)
{
    hasColors = false;
    int val = 0;
    val += Genhdr::bcolor.Load(bcolor);
    val += Genhdr::pcolor.Load(pcolor);
    val += Genhdr::fcolor.Load(fcolor);
    val += Genhdr::scolor.Load(scolor);
    if (val)
    {
        ERRBLURB;
        cerr << "problems loading color specifications\n";
        return -1;
    }
    hasColors = true;
    return 0;
}



int Genhdr::SetCase(int col, int row, double colpitch, double rowpitch,
        double height, double shoulder, bool hassh, double bevel)
{
    hasBparams = false;

    if (col < 1)
    {
        ERRBLURB;
        cerr << "invalid number of columns (" << col << "); must be >= 1\n";
        return -1;
    }
    cols = col;

    if (row < 1)
    {
        ERRBLURB;
        cerr << "invalid number of rows (" << row << "); must be >= 1\n";
        return -1;
    }
    rows = row;

    if (colpitch <= 0.0)
    {
        ERRBLURB;
        cerr << "invalid column pitch (<= 0)\n";
        return -1;
    }
    xp = colpitch;

    if (rowpitch <= 0.0)
    {
        ERRBLURB;
        cerr << "invalid row pitch (<= 0)\n";
        return -1;
    }
    yp = rowpitch;

    if (height < 0.3)
    {
        ERRBLURB;
        cerr << "invalid height (<0.3)\n";
        return -1;
    }
    bh = height;

    if (shoulder > (height - 0.3))
    {
        ERRBLURB;
        cerr << "invalid shoulder height (> (height -0.3))\n";
        return -1;
    }
    sh = shoulder;
    Genhdr::hassh = hassh;

    if ((bevel > colpitch/3.0) || (bevel > rowpitch/3.0))
    {
        ERRBLURB;
        cerr << "invalid bevel (> 1/3 row or column pitch)\n";
        return -1;
    }
    bev = bevel;

    hasBparams = true;
    return 0;
}



int Genhdr::SetPins(bool squarebot, bool squaretop, bool male, double pbev,
        double fbev, double depth, double length, double pd0, double pdy, double pd1,
        double pd2, double pd3, double ftc, double taper, double ts, int sides, double funneldepth)
{
    hasPparams = false;

    Genhdr::male = male;

    if (depth < 0.0)
    {
        ERRBLURB;
        cerr << "invalid pin depth (< 0)\n";
        return -1;
    }
    pd = depth;

    // note: cannot completely check until Calc() where case height is also known
    if (length < depth)
    {
        ERRBLURB;
        cerr << "invalid pin length (< depth)\n";
        return -1;
    }
    pl = length;

    if (pd0 < 1e-9)
    {
        ERRBLURB;
        cerr << "invalid pd0 (<= 0)\n";
        return -1;
    }

    if (squarebot)
    {
        if (pdy < pd0/10.0)
        {
            ERRBLURB;
            cerr << "pdy must be >= pd0/10\n";
            return -1;
        }
        Genhdr::pdy = pdy;
        if ((pbev > pd0/4.0) || (pbev > pdy/4.0))
        {
            ERRBLURB;
            cerr << "pin bevel (pbev) must be <= min(pd0, pdy)/4.0\n";
            return -1;
        }
        Genhdr::pbev = pbev;
    }
    Genhdr::pd0 = pd0;
    if (male)
    {
        squaretop = squarebot;
    }
    else
    {
        if (pd1 < pd0)
        {
            ERRBLURB;
            cerr << "invalid pd1 (< pd0)\n";
            return -1;
        }
        Genhdr::pd1 = pd1;
        if (!squaretop)
        {
            if (ftc < 1.0)
            {
                ERRBLURB;
                cerr << "invalid ftc (must be >= 1.0)\n";
                return -1;
            }
            Genhdr::ftc = ftc;
            if (pd2 > pd1/ftc)
            {
                ERRBLURB;
                cerr << "invalid pd2 (> pd1/ftc)\n";
                return -1;
            }
        }
        Genhdr::pd2 = pd2;
        if ((!squarebot) && (pd3 < pd0))
        {
            ERRBLURB;
            cerr << "invalid pd3 (< pd0)\n";
            return -1;
        }
        Genhdr::pd3 = pd3;
        if ((squaretop) && (fbev > (pd1 - pd2)/2.0))
        {
            ERRBLURB;
            cerr << "fbev must be <= (pd1 - pd2)/2.0\n";
            return -1;
        }
        Genhdr::fbev = fbev;
        if (!squarebot)
        {
            Genhdr::pdy = pd0;
        }
    }

    Genhdr::squarebot = squarebot;
    Genhdr::squaretop = squaretop;

    if (taper >= depth)
    {
        ERRBLURB;
        cerr << "invalid pin taper (>= depth)\n";
        return -1;
    }
    pt = taper;

    if ((ts < 0.0) || (ts > 1.0))
    {
        ERRBLURB;
        cerr << "invalid taper ratio (<0 or >1)\n";
        return -1;
    }
    pts = ts;

    if ((!squarebot) || (!squaretop))
    {
        if ((sides < 4) || (sides > 360) || (sides %4))
        {
            ERRBLURB;
            cerr << "invalid number of sides (" << sides << "); must be 4..360 and a multiple of 4\n";
            return -1;
        }
        ns = sides;
    }

    fd = funneldepth;

    hasPparams = true;
    return 0;
}

// render the case
int Genhdr::makeCase(kc3d::Transform &t, std::ofstream &fp, int tabs)
{
    int val = 0;
    Hdrbase hbase;

    double ch = bh;
    if ((!male) && (fd < 0.0)) ch += fd;

    if ((squarebot) || (male))
    {
        val += hbase.SetParams(xp, yp, bev, ch, sh, hassh, pd0, pdy, pd1,
                squarebot, squaretop, male, pbev, fbev, cols, rows, ns);
    }
    else
    {
        val += hbase.SetParams(xp, yp, bev, ch, sh, hassh, pd3, pd3, pd1,
                squarebot, squaretop, male, pbev, fbev, cols, rows, ns);
    }

    if (val)
    {
        ERRBLURB;
        cerr << "could not set parameters for case\n";
        return -1;
    }

    val += hbase.Build(t, bcolor, false, fp, tabs);

    if (val) return -1;
    return 0;
}


// render the pins
int Genhdr::makePins(kc3d::Transform &t, std::ofstream &fp, int tabs)
{
    int val = 0;

    Transform t0, t1;
    Pin pin[2];

    pin[0].SetShape(squarebot);
    pin[1].SetShape(squarebot);

    PParams p0, p1;
    double tpl; // total pin length (pin[0])
    double tpo, tph, tpt, tps; // second pin's offset, height, taper, and taper ratio
    double dtmp;
    if (male)
    {
        tpl = pl;
    }
    else
    {
        tpl = pd;
        if (sh > 0)
        {
            if (squarebot)
            {
                tpl += sh;
            }
            else
            {
                // ensure that the angle is <= 45 deg
                dtmp = sh/3.0;
                if (dtmp > ((pd3 - pd0)/2.0)) dtmp = (pd3 - pd0)/2.0;
                tpo = 0.0;
                tph = sh;
                tpt = dtmp;
                tps = pd0/pd3;
            }
        }
    }

    // compute the pins
    p0.bend = -1;
    p0.w = pd0;
    if (male)
    {
        p0.dbltap = true;
        p0.bev = pbev;
        p0.d = pdy;
    }
    else
    {
        p0.dbltap = false;
        if (squarebot)
        {
            p0.d = pdy;
            p0.bev = pbev;
        }
        else
        {
            p0.d = pd0;
            p0.bev = -1;
        }
    }
    p0.tap = pt;
    p0.h = tpl;
    p0.l = -1;
    p0.ns = ns;
    p0.std = pts;
    p0.stw = pts;
    if ((!male)&&(!squarebot))
    {
        p1.bend = -1;
        p1.bev = -1;
        p1.d = pd3;
        p1.w = pd3;
        p1.dbltap = false;
        p1.tap = tpt;
        p1.h = tph;
        p1.l = -1;
        p1.ns = ns;
        p1.std = tps;
        p1.stw = tps;
    }

    int i, j;
    double ox, oy, oxb, oyb;    // offset information
    ox = (1 - cols)*xp/2.0;
    oy = (1 - rows)*yp/2.0;

    bool reuse_pc = false;
    for (i = 0; i < rows; ++ i)
    {
        oyb = oy + i*yp;
        for (j = 0; j < cols; ++j)
        {
            oxb = ox + j*xp;
            t0.setTranslation(oxb, oyb, -pd);
            val += pin[0].Calc(p0, t0);
            if (male)
                val += pin[0].Build(true, true, t, pcolor, reuse_pc, fp, tabs);
            else
                val += pin[0].Build(true, false, t, pcolor, reuse_pc, fp, tabs);
            reuse_pc = true;
            if ((!male) && (!squarebot))
            {
                t1.setTranslation(oxb, oyb, tpo);
                val += pin[1].Calc(p1, t1);
                val += pin[1].Build(false, false, t, pcolor, true, fp, tabs);
            }
        }
    }

    if (val)
    {
        ERRBLURB;
        cerr << "problems writing pin information to file\n";
        return -1;
    }
    return 0;
}

// render shrouds for circular pins in female headers
int Genhdr::makeShrouds(kc3d::Transform &t, std::ofstream &fp, int tabs)
{
    // relevant parameters:
    // pd1;     // pin diameter 1 (Female, pin dia. within header)
    // pd2;     // pin diameter 2 (Female, dia. of funnel)
    // ns;      // number of sides to a circule
    // fd;      // funnel depth (for female headers)
    // bh;      // case height

    // circle[0] : edge of header casing
    // circle[1] : depressed circle surrounding funnel
    // circle[2] : inner circle 1, planar with circle[1]
    // circle[3] : inner circle 2, planar with circle[1]
    Circle circ[4];

    Transform t0, t1;

    int i, j;
    int val = 0;
    double td1 = pd2*ftc;
    double td0 = td1 + (pd1 - td1)*0.4;
    double ox, oy, oxb, oyb;
    ox = (1 - cols)*xp/2.0;
    oy = (1 - rows)*yp/2.0;

    double ch = bh; // case height
    bool raised = false;
    if (!male)
    {
        if (fd < -1e-9)
        {
            raised = true;
            ch = bh + fd;
        }
        if (fd > 1e-9) raised = true;
    }

    bool reuse = false;
    if (!bcolor.GetName().compare(scolor.GetName())) reuse = true;
    if (!pcolor.GetName().compare(scolor.GetName())) reuse = true;

    for (i = 0; i < 4; ++i) circ[i].SetNVertices(ns);

    for (i = 0; i < rows; ++ i)
    {
        oyb = oy + i*yp;
        for (j = 0; j < cols; ++j)
        {
            oxb = ox + j*xp;
            t0.setTranslation(oxb, oyb, ch);
            t1.setTranslation(oxb, oyb, ch - fd);
            circ[0].Calc(pd1, pd1, t0);
            circ[1].Calc(pd1, pd1, t1);
            circ[2].Calc(td0, td0, t1);
            circ[3].Calc(td1, td1, t1);
            if (raised) val += circ[0].Stitch(true, circ[1], t, scolor, reuse, fp, tabs);
            val += circ[1].Stitch(true, circ[2], t, scolor, true, fp, tabs);
            val += circ[2].Stitch(true, circ[3], t, pcolor, true, fp, tabs);
        }
    }
    if (val)
    {
        ERRBLURB;
        cerr << "problems writing shroud data to file\n";
        return -1;
    }

    return 0;
}

// render funnels for female headers
int Genhdr::makeFunnels(kc3d::Transform &t, std::ofstream &fp, int tabs)
{
    // relevant parameters:
    // pd1;     // pin diameter 1 (Female, pin dia. within header)
    // pd2;     // pin diameter 2 (Female, dia. of funnel)
    // ns;      // number of sides to a circule
    // fd;      // funnel depth (for female headers)
    // bh;      // case height

    Funnel fun; // circular or square funnel
    int val = 0;
    // set the parameters based on square/circle and funnel depth
    double fdia;            // funnel diameter
    double fh0, fh1, fh2;   // funnel flute and stem heights
    double fz;              // z offset for funnels
    VRMLMat *f0col, *f1col;

    double tvar = bh;   // room for the funnel

    // calculate the max allowable recess
    if (sh > 1e-9)
    {
        if (squarebot)
        {
            tvar -= sh;
        }
        else
        {
            double so;
            so = sh/3.0;
            if (so > ((pd3 - pd0)/2.0)) so = (pd3 - pd0)/2.0;
            tvar -= so;
        }
    }

    bool reuse_f0col = false;
    bool reuse_f1col = false;
    if (squaretop)
    {
        fun.SetShape(true, fbev);
        fdia = pd1;
        fz = bh;
        fh0 = fd;
        tvar -= fd;
        tvar *= 0.9;
        fh1 = fd/2.0;
        if (fh1 > tvar) fh1 = tvar/3.0;
        fh2 = tvar - fh1;
        f0col = &bcolor;
        f1col = &fcolor;
    }
    else
    {
        fun.SetShape(false, -1.0);
        if (fd > 1e-9) tvar -= fd;
        fdia = pd2*ftc;
        if (fd < -1e-9)
            fz = bh;
        else
            fz = bh - fd;
        // fh0 is nominally calculated for a 45 deg slope
        fh0 = pd2*(ftc - 1.0)/2.0;
        tvar *= 0.9;
        if (fh0 >= tvar) fh0 = tvar/3.0;
        fh1 = 0.0;
        fh2 = tvar - fh0;
        f0col = &fcolor;
        f1col = &fcolor;
    }
    // conditions under which we reuse the colors
    if (!f0col->GetName().compare(bcolor.GetName())) reuse_f0col = true;
    if (!f0col->GetName().compare(pcolor.GetName())) reuse_f0col = true;
    if ((!f0col->GetName().compare(scolor.GetName())) && (!male) && (!squaretop)) reuse_f0col = true;
    if (!f1col->GetName().compare(bcolor.GetName())) reuse_f1col = true;
    if (!f1col->GetName().compare(pcolor.GetName())) reuse_f1col = true;
    if ((!f1col->GetName().compare(scolor.GetName())) && (!male) && (!squaretop)) reuse_f1col = true;

    Transform t0;
    int i, j;
    double ox, oy, oxb, oyb;    // offset information
    ox = (1 - cols)*xp/2.0;
    oy = (1 - rows)*yp/2.0;

    for (i = 0; i < rows; ++ i)
    {
        oyb = oy + i*yp;
        for (j = 0; j < cols; ++j)
        {
            oxb = ox + j*xp;
            t0.setTranslation(oxb, oyb, fz);
            val += fun.Calc(fdia, fdia, pd2, pd2, fh0, fh1, fh2, t0, ns);
            val += fun.Build(true, t, *f0col, reuse_f0col, *f1col, reuse_f1col, fp, tabs);
            reuse_f0col = true;
            reuse_f1col = true;
        }
    }

    if (val)
    {
        ERRBLURB;
        cerr << "problems writing funnel information to file\n";
        return -1;
    }

    return 0;
}

}   // namespace kc3dconn
