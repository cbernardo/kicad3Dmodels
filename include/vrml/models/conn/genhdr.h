/*
 *      file: genhdr.h
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

/*
 * This model represents a generic PCB header
 * Parameters control:
 *      square or round pins
 *      male or female
 *      height
 *      shoulder height
 *      bevel
 *      xpitch
 *      ypitch
 *      columns
 *      rows (supports > 2 rows although this is probably not needed)
 *      hole dimension
 *      second hole dimension for round female headers
 *
 *      body color
 *      pin color
 *      funnel color for round pins
 *
 */



#ifndef GENHDR_H_
#define GENHDR_H_

#include <iosfwd>

#include "vrmlmat.h"
#include "transform.h"

namespace kc3dconn {

class Genhdr
{
private:
    kc3d::VRMLMat bcolor; // body (casing) color
    kc3d::VRMLMat pcolor; // pin color
    kc3d::VRMLMat fcolor; // funnel color (for female hdr only)

    int cols;       // columns; must be >= 2
    int rows;       // rows; must be >= 1
    double xp;      // pitch for columns
    double yp;      // pitch for rows
    double sh;      // shoulder height
    double bh;      // body height
    double bev;     // bevel

    bool square;    // true for square pins
    bool male;      // true if male header
    double pd;      // pin depth (below top of board)
    double pl;      // pin length (primarily for male pin)
    double pd0;     // pin diameter 0 (or length of side)
    double pd1;     // pin diameter 1 (Female, pin dia. within header)
    double pd2;     // pin diameter 2 (Female, dia. of funnel)
    double pt;      // pin taper
    double pts;     // pin taper ratio
    int ns;         // number of sides to a circular pin (must be 4*N, 4 .. 360)
    double fd;      // funnel depth (for female headers)

    // book keeping
    bool hasColors;     // true when colors have been loaded
    bool hasBparams;    // true when body parameters have been set
    bool hasPparams;    // true when pin parameters have been set

    // render the case
    int makeCase(kc3d::Transform &t, std::ofstream &fp, int tabs = 0);
    // render the pins
    int makePins(kc3d::Transform &t, std::ofstream &fp, int tabs = 0);
    // render shrouds for circular pins in female headers
    int makeShrouds(kc3d::Transform &t, std::ofstream &fp, int tabs = 0);
    // render funnels for female headers
    int makeFunnels(kc3d::Transform &t, std::ofstream &fp, int tabs = 0);

public:
    Genhdr();

    int Build(kc3d::Transform &t, std::string part, std::ofstream &fp, int tabs = 0);

    int SetColors(std::string bcolor, std::string pcolor, std::string fcolor);

    int SetCase(int col, int row, double colpitch, double rowpitch,
            double height, double shoulder, double bevel);

    int SetPins(bool square, bool male, double depth, double length,
            double pd0, double pd1, double pd2, double taper, double ts,
            int sides, double funneldepth);
};

}   // namespace kc3dconn

#endif /* GENHDR_H_ */
