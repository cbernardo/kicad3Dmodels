/*
 file: dip_pin.h

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

 */

#ifndef DIP_PIN_H
#define DIP_PIN_H

#include <iosfwd>

#include "transform.h"

#define PIN_NP (52)

class dippin
{
private:
    Quat p[PIN_NP];
    double BEV;     ///< pin edge bevel (not yet exposed)
    double scale;   ///< world scale parameter; needed to keep KiCad happy
    bool valid;     ///< true when the computed vertices are valid
    bool ismetric;  ///< true if user wishes to create metric parts
    /* pin parameters; defaults are for PDIP-24, inches */
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

public:
    dippin();
    /// select metric units or inches
    void setMetric(bool metric);

    /// set the virtual world's scaling parameter
    void setWorldScale(double sc)
    {
        scale = sc;
        valid = false;
    }
    /// retrieve the virtual world's scaling parameter
    double getWorldScale(void)
    {
        return scale;
    }

    /// set the base height of the case above the PCB
    int setBaseHeight(double a1);
    /// retrieve the base height of the case above the PCB
    double getBaseHeight(void)
    {
        return A1;
    }
    /// set the depth of the case
    int setCaseDepth(double a2);
    /// retrieve the depth of the case
    double getCaseDepth(void)
    {
        return A2;
    }
    /// set the breadth (widest portion) of the pin
    int setPinBreadth(double b1);
    /// retrieve the breadth of the pin
    double getPinBreadth(void)
    {
        return B1;
    }
    /// set the THRU width of the pin
    int setThruWidth(double b);
    /// retrieve the THRU width of the pin
    double getThruWidth(void)
    {
        return B;
    }
    /// set the height of the tapered portion of the pin
    int setTaperHeight(double b2);
    /// retrieve the height of the tapered portion of the pin
    double getTaperHeight(void)
    {
        return B2;
    }
    /// set the length of the dogeared portion of the pin
    int setDogEar(double c1);
    /// retrieve the length of the dogeared portion of the pin
    double getDogEar(void)
    {
        return C1;
    }
    /// set the lower dogear's offset
    int setDogEarOffset(double c2);
    /// retrieve the lower dogear's offset
    double getDogEarOffset(void)
    {
        return C2;
    }
    /// set the material thickness of the pin
    int setThickness(double c);
    /// retrieve the material thickness of the pin
    double getThickness(void)
    {
        return C;
    }
    /// set the width of the case
    int setCaseWidth(double e1);
    /// retrieve the width of the case
    double getCaseWidth(void)
    {
        return E1;
    }
    /// set the spacing of the pin rows
    int setRowSpacing(double e);
    /// retrieve the spacing of the pin rows
    double getRowSpacing(void)
    {
        return E;
    }
    /// set the length of the THRU portion of the pin
    int setThruLength(double l);
    /// retrieve the length of the THRU portion of the pin
    double getThruLength(void)
    {
        return L;
    }

    /// calculate the vertices
    Quat *calc(void);
    /// write the point list as a VRML Coordinate{}
    int writeCoord(std::ofstream &fp, int tabs = 0, Transform *t = NULL);
    /// write the facet list as a VRML coordIndex[]
    int writeFacets(std::ofstream &fp, int tabs);
};

#endif /* DIP_PIN_H */
