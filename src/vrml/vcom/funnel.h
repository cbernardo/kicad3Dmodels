/*
 *      file: funnel.h
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
 *      This class represents a rectangular or circular funnel which can
 *      be used to create recessed pins.
 *
 *             _______
 *            /      / = d1
 *           /__w1__/
 *           \     /  = h1
 *            \___/
 *            |   | | = h2
 *            |___|/|
 *            |   | |
 *            |   | | = h3
 *            |   | |
 *            |_w2|/  = d2
 *
 *            w1, d1 = x, y dimensions of top of flute
 *            w2, d2 = x, y dimensions of stem
 *            h1 = height of flute
 *            h2 = height of stem which will be FluteColor
 *            h3 = height of stem which will be StemColor
 *            FluteColor = color of the flute
 *            StemColor  = color of the stem
 *            (bool) cap = true if the stem is to be capped at the bottom
 *
 *      Sample parameters:
 *          w1 = 1.8;
 *          d1 = 1.8;
 *          w2 = 0.9;
 *          d2 = 0.9;
 *          h1 = 0.7;
 *          h2 = 0.7;
 *          h3 = 4.0;
 *
 */

#ifndef FUNNEL_H_
#define FUNNEL_H_

#include <iosfwd>
#include "polygon.h"

class VRMLMat;
class Transform;

class Funnel
{
private:
    bool   valid;
    bool   square;
    int    npoly;       // number of polygons in the funnel
    double bev;         // bevel for square funnels
    Polygon **poly;     // list of polygons

    void cleanup(void);


public:
    Funnel();
    Funnel (const Funnel &p);
    virtual ~Funnel();

    Funnel &operator=(const Funnel &p);

    int Calc(double w1, double d1, double w2, double d2,
            double h1, double h2, double h3, Transform &t, int ns = 16);

    int Build(bool cap, Transform &t, VRMLMat &flutecolor, bool reuse_flute,
            VRMLMat &stemcolor, bool reuse_stem, std::ofstream &fp, int tabs = 0);

    void SetShape(bool square, double bev = -1.0);
};

#endif /* FUNNEL_H_ */
