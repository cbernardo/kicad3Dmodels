/*
  file: dip.h

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

  Notes:
    These classes were created to support the
    programmatic creation of VRML models for
    use with KiCad.

    This class represents a parameterized DIP 3D model.

  NOTE: This is very bare at the moment

*/

#ifndef DIP_H
#define DIP_H

#include <iosfwd>
#include <string>

#include "vrmlmat.h"

struct dipparams
{
    bool   metric;  ///< true if we wish to calculate with metric dimensions
    double scale;   ///< World Scale; set to '10' for KiCAD representation (0.1inch = 1VRML unit)
    double pinbev;  ///< Pin edge bevel
    unsigned short npins;   ///< number of pins in the package

    double A1;      ///< [BaseHeight] distance from board to bottom of case
    double A2;      ///< [CaseDepth]
    double B;       ///< [ThruWidth]
    double B1;      ///< [PinBreadth] widest part of the pin
    double B2;      ///< [TaperHeight] height of the pin's tapered portion
    double C1;      ///< [DogEar] length of top bent (sloped) portion)
    double C2;      ///< [DogEarOffset] offset of lower bent (sloped) portion)
    double C;       ///< [Thickness] pin material thickness
    double E1;      ///< [CaseWidth]
    double E;       ///< [RowSpacing] pin row spacing (for example, 0.3 inches on typical DIP)
    double L;       ///< [ThruLength] pin length (narrow part)
    double P;       ///< [PinPitch]

    double NW;      ///< [NotchWidth]
    double ND;      ///< [NotchDepth]
    double casebev; ///< [CaseBevel]
    double MID;     ///< [CaseMidHeight] depth of middle portion where pins attach
    double NL;      ///< [NotchLength] must be > NW/2
    double D;       ///< [CaseLength]
    double S;       ///< [CaseTaper] deviation of unbeveled top and bottom edges
    double DW;      ///< [CaseAlw]  allowance on case length

    VRMLMat pinmaterial;
    VRMLMat casematerial;

    dipparams();
};

class dip {
  private:
    dipparams   params;

  public:
    dip();
    int Calc(int pins, std::string filename);
    int SetParams(const dipparams &p);
};

#endif
