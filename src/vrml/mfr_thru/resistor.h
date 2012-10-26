/*
 *      file: resistor.h
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
 *      Notes on color coding:
 *
 *      + values 0..9 are blk, brn, red, org, yel, grn, blu, pur, gry, wht
 *      + multiplier may be Gold (*.1) or Silver (*0.01)
 *      + tolerances are:           : code for use in filename
 *          20%   : no band         : no code; fill with '-'
 *          10%   : silver  ('S')   : K
 *          5%    : gold    ('G')   : J
 *          2%    : red     ('2')   : G
 *          1%    : brn     ('1')   : F
 *          0.5%  : grn     ('5')   : D
 *          0.25% : blu     ('6')   : C
 *          0.1%  : pur     ('7')   : B
 *          0.05% : gry     ('8')   : W
 *      + temperature code (ppm/K):
 *          250   : blk             : U
 *          100   : brn             : S
 *          50    : red             : R
 *          15    : org             : P
 *          25    : yel             : Q
 *          20    : grn             : Z  -- Z = no color assigned in EIC 60062;
 *                                          coeff. specified in other documentation)
 *          10    : blu             : Z
 *          5     : pur             : M
 *          1     : gry             : K
 *      + The large gap is always between the value and multiplier bands
 *
 *  From: http://www.ebyte.it/library/educards/ee/EE_ColorCodes.html
 *  Occasionally, one can encounter resistors with either six color bands
 *  or resistors with five bands, the last one of which the last one is
 *  anomalous for a tolerance class specification (orange, yellow or white).
 *  In such cases, the last band defines the worst-case temperature-dependence
 *  coefficient of the component.
 */

/*
 *  This is a parametric representation of a resistor. Inputs include:
 *      + world scale
 *      + Length of body
 *      + Diameter of body
 *      + Diameter of wire
 *      + Lead pitch
 *      + Orientation (horiz. or vert.)
 *      + Color code: 4 to 6 characters to represent color;
 *          color characters are 0..9 for value codes and tolerance,
 *          'G' for Gold tolerance, 'S' for Silver tolerance, 'X' for
 *          no band. The last character must be the tolerance; if no
 *          tolerance band, use 'X'. For example, a 0-ohm resistor
 *          would have the code 'XX0XX', 1.7k 5% = '172XG' to produce
 *          a 4-band code with a gap between the value and tolrance codes,
 *          1.69k 1% = '16911' (no gap between value and tolerance),
 *          200-ohm 20% = '201X'.
 *      + Body color
 *      + End finish (cap, round, bulge)
 *      + sides on wire (8..360, 16-24 should be good)
 *      + sides in bend (4..24, 4-6 should be OK)
 *      + sides on resistor (3..360 - 48 or 96 are good choices)
 *
 *  Internal parameters:
 *      + bend radius is fixed at 1.5*wire diameter
 *      + wire color is fixed to 'tin.mat'
 *      + small metallic caps on ends are 1.0*wire diameter thick
 *          and 2*wire in diameter
 *      + resistor body is 0.5*wire diameter from the board surface
 *      + reference point is the left lead; body extends to the right
 *      + for a vertical orientation, the bend radius at the top = pitch/2
 *          with a straight lead of 2*wire dia.
 */

#ifndef RESISTOR_H_
#define RESISTOR_H_

#include <iosfwd>
class VRMLMat;

struct RParams
{
    double scale;   // world scale
    double shift;   // shift in x-axis to control (0,0) reference point (default 0)
    double L;       // length of body
    double D;       // diameter of body
    double d;       // diameter of wire
    double p;       // lead pitch
    bool horiz;     // orientation; true if horizontal
    char endshape;  // end finish; 'C'ap, 'R'ound, 'B'ulge (default)
    bool bcap;      // true to place a metallic cap on a Bulge style end (default false)
    int wsides;     // number of sides to the wire (default 16)
    int bsides;     // number of sides in a 90-deg bend (default 6)
    int rsides;     // number of sides on the resistor (default 48)
    std::string pwrsuf;     // suffix for power rating
    std::string spcsuf;     // suffix for pin spacing
    VRMLMat colors[14]; // color values for 0..9, Gold, Silver, body color, wire color

    RParams();
};

class Resistor
{
    RParams params;
    int makeHzLead(std::ofstream &fp);
    int startVtLead(std::ofstream &fp);
    int endVtLead(std::ofstream &fp);
    int makeBody(std::ofstream &fp, const std::string &bands);
    int validateBands(const std::string &bands);
    int getColorIdx(int i, const std::string &bands);

public:
    int Create(RParams &rp, const std::string &bands, const std::string &filename);
    // filename = base filename =
    // [Part Series]_[Value]_[Tolerance][Tcoeff][End Style][Orientation]_[Power]_[Lead Space]

};


#endif /* RESISTOR_H_ */
