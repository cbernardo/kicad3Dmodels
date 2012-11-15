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
    val += bcolor.WriteMaterial(fp, tabs, true);
    val += pcolor.WriteMaterial(fp, tabs, true);
    if (!male) val += fcolor.WriteMaterial(fp, tabs, true);
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

    if ((!male) && (!square)) val += makeShrouds(t, fp, tabs+2);
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



int Genhdr::SetColors(std::string bcolor, std::string pcolor, std::string fcolor)
{
    hasColors = false;
    int val = 0;
    val += Genhdr::bcolor.Load(bcolor);
    val += Genhdr::pcolor.Load(pcolor);
    val += Genhdr::fcolor.Load(fcolor);
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
        double height, double shoulder, double bevel)
{
    hasBparams = false;

    if (col < 2)
    {
        ERRBLURB;
        cerr << "invalid number of columns (" << col << "); must be >= 2\n";
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



int Genhdr::SetPins(bool square, bool male, double depth, double length,
        double pd0, double pd1, double pd2, double taper, double ts,
        int sides, double funneldepth)
{
    hasPparams = false;

    Genhdr::square = square;
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
    Genhdr::pd0 = pd0;

    if (!male)
    {
        if (pd1 < pd0)
        {
            ERRBLURB;
            cerr << "invalid pd1 (< pd0)\n";
            return -1;
        }
        Genhdr::pd1 = pd1;
        if (pd2 > pd1)
        {
            ERRBLURB;
            cerr << "invalid pd2 (> pd1)\n";
            return -1;
        }
        Genhdr::pd2 = pd2;
    }

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

    if (!square)
    {
        if ((sides < 4) || (sides > 360) || (sides %4))
        {
            ERRBLURB;
            cerr << "invalid number of sides (" << sides << "); must be 4..360 and a multiple of 4\n";
            return -1;
        }
        ns = sides;
    }

    if (!male)
    {
        if (funneldepth < 0.0)
        {
            ERRBLURB;
            cerr << "invalid funnel depth (< 0)\n";
            return -1;
        }
        fd = funneldepth;
    }

    hasPparams = true;
    return 0;
}

// render the case
int Genhdr::makeCase(kc3d::Transform &t, std::ofstream &fp, int tabs)
{
    int val = 0;
    Hdrbase hbase;

    if (male)
        val += hbase.SetParams(xp, yp, bev, bh, sh, pd0, pd0, square, cols, rows, ns);
    else
        val += hbase.SetParams(xp, yp, bev, bh, sh, pd0, pd1, square, cols, rows, ns);
    if (val)
    {
        ERRBLURB;
        cerr << "could not set parameters for case\n";
        return -1;
    }

    val += hbase.Build(t, bcolor, true, fp, tabs);

    if (val) return -1;
    return 0;
}


// render the pins
int Genhdr::makePins(kc3d::Transform &t, std::ofstream &fp, int tabs)
{
    int val = 0;

    Transform t0, t1;
    Pin pin[2];

    if (!square)
    {
        pin[0].SetShape(false);
        pin[1].SetShape(false);
    }

    PParams p0, p1;
    double tpl; // total pin length (pin[0])
    double tpo, tph, tpt, tps; // second pin's offset, height, taper, and taper ratio
    if (male)
    {
        tpl = pl;
    }
    else
    {
        tpl = pd;
        if (sh > 0)
        {
            if (square)
            {
                tpl += sh;
            }
            else
            {
                tpl += sh/3.0;
                tpo = sh/3.0;
                tph = tpo*2.0;
                tpt = tpo;
                tps = pd0/pd1;
            }
        }
    }

    // compute the pins
    p0.bend = -1;
    p0.bev = -1;
    p0.d = pd0;
    p0.w = pd0;
    if (male)
        p0.dbltap = true;
    else
        p0.dbltap = false;
    p0.tap = pt;
    p0.h = tpl;
    p0.l = -1;
    p0.ns = ns;
    p0.std = pts;
    p0.stw = pts;
    if ((!male)&&(!square))
    {
        p1.bend = -1;
        p1.bev = -1;
        p1.d = pd1;
        p1.w = pd1;
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

    for (i = 0; i < rows; ++ i)
    {
        oyb = oy + i*yp;
        for (j = 0; j < cols; ++j)
        {
            oxb = ox + j*xp;
            t0.setTranslation(oxb, oyb, -pd);
            val += pin[0].Calc(p0, t0);
            if (male)
                val += pin[0].Build(true, true, t, pcolor, true, fp, tabs);
            else
                val += pin[0].Build(true, false, t, pcolor, true, fp, tabs);
            if ((!male) && (!square))
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
    // circle[2] : inner circle, planar with circle[1]
    Circle circ[3];

    Transform t0, t1;

    int i, j;
    int val = 0;
    double td = pd2 + (pd1 - pd2)/3.0;
    double ox, oy, oxb, oyb;
    ox = (1 - cols)*xp/2.0;
    oy = (1 - rows)*yp/2.0;

    for (i = 0; i < rows; ++ i)
    {
        oyb = oy + i*yp;
        for (j = 0; j < cols; ++j)
        {
            oxb = ox + j*xp;
            t0.setTranslation(oxb, oyb, bh);
            t1.setTranslation(oxb, oyb, bh - fd);
            circ[0].Calc(pd1, pd1, t0);
            circ[1].Calc(pd1, pd1, t1);
            circ[2].Calc(td, td, t1);
            if (fd > 1e-9) val += circ[0].Stitch(true, circ[1], t, bcolor, true, fp, tabs);
            val += circ[1].Stitch(true, circ[2], t, pcolor, true, fp, tabs);
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
    double fh0, fh1, fh2;   // funnel bits
    double fz;              // z offset for funnels
    VRMLMat *f0col, *f1col;

    double tvar = bh;   // room for the funnel
    if (sh > 1e-9) tvar -= sh;

    if (square)
    {
        fun.SetShape(true, -1.0);
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
        tvar -= fd;
        fdia = pd2 + (pd1 - pd2)/3.0;
        fz = bh - fd;
        fh0 = pd2/3.0;
        tvar *= 0.9;
        if (fh0 >= tvar) fh0 = tvar/3.0;
        fh1 = 0.0;
        fh2 = tvar - fh0;
        f0col = &fcolor;
        f1col = &fcolor;
    }

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
            val += fun.Build(true, t, *f0col, true, *f1col, true, fp, tabs);
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
