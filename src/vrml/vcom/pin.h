/*
 *      file: pin.h
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

#ifndef PIN_H_
#define PIN_H_

#include <iosfwd>

class Polygon;
class PolyRect;
class VRMLMat;
class Transform;


struct PParams
{
    double w;       // pin width
    double d;       // pin depth
    double tap;     // pin taper, < 0 to indicate no taper
    double stw;     // scale factor for taper in 'w' dimension
    double std;     // scale factor for taper in 'd' dimension
    bool   dbltap;  // true if tapered at both ends
    double h;       // pin height (vertical section)
    double r;       // pin bend radius, < 0 to indicate no bend (and no hz section)
    int    nb;      // number of segments in a bend (default 5)
    double l;       // length of horizontal section, < 0 for no hz section
    double bev;     // bevel where applicable; < 0 means no bevel
    double bend;    // bend angle (radians)
    int    ns;      // number of sides for a circular pin (default 24)

    PParams();
};

class Pin
{
private:
    Polygon *poly;     // pointer to rectangles
    int nr;             // number of polygons
    bool valid;         // true when we have valid polygons
    bool square;        // true (default) for square pins

protected:
    PParams pin;

public:
    Pin();
    virtual ~Pin();

    // Calculate the intermediate polygons
    virtual int Calc(const PParams &pp, Transform &t);

    // Write out the pin shape information
    virtual int Build(bool cap0, bool cap1, Transform &t, VRMLMat &color, bool reuse_color,
            std::ofstream &fp, int tabs = 0);

    // set the shape of the generic pin
    void SetShape(bool square);
};

#endif /* PIN_H_ */
