/*
 *      file: hole.h
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
 *      This object represents a rectangular frame with a hole in it.
 *      The hole may be rectangular or elliptical and may be offset.
 */

#ifndef HOLE_H_
#define HOLE_H_

#include <iosfwd>

class Polygon;
class Transform;
class VRMLMat;

class Hole
{
private:
    bool valid;
    bool square;
    double w1, w2;  // width of frame, hole
    double d1, d2;  // depth of frame, hole
    double ow, od;  // hole offset
    Polygon *hole;  // hole (circular or rectangular)
    double v[3][4]; // vertices for the rectangular frame
    int np;         // number of points in a circular hole

    int writeRFacets(bool top, std::ofstream &fp, int tabs = 0);

    int writeFacets(int v0, int v1, int h0, int h1, int lp, bool top,
            std::ofstream &fp, int tabs = 0);

    // Hide the default copy and assignment operators
    Hole(const Hole &p);
    Hole &operator=(const Hole &p);

public:
    Hole();
    virtual ~Hole();

    int Calc(double w1, double d1, double w2, double d2, Transform &tx,
            bool square, double ow = 0.0, double od = 0.0, int np = 16);

    int Build(bool top, Transform &t, VRMLMat &color, bool reuse,
            std::ofstream &fp, int tabs = 0);

};

#endif /* HOLE_H_ */
