/*
 file: dip_case.h

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

#ifndef DIP_CASE_H
#define DIP_CASE_H

#include <iosfwd>

#include "transform.h"

#define CASE_NP (76)

class dipcase
{
private:
    Quat p[CASE_NP];
    double NW;      ///< [NotchWidth]
    double ND;      ///< [NotchDepth]
    double BEV;     ///< [CaseBevel]
    double MID;     ///< [CaseMidHeight] depth of middle portion where pins attach
    double NL;      ///< [NotchLength] must be > NW/2
    double scale;   ///< [WorldScale] conversion scale from real units to virtual world units
    double D;       ///< [CaseLength]
    double E1;      ///< [CaseWidth]
    double S;       ///< [CaseTaper] deviation of unbeveled top and bottom edges
    double A1;      ///< [BaseHeight] distance from board to bottom of case
    double A2;      ///< [CaseDepth]
    bool valid;     ///< true when the current set of vertices are valid
    bool ismetric;  ///< true when the user wishes to perform metric calculations

public:
    dipcase();
    /// select metric units or inches
    void setMetric(bool metric);

    /* case parameters; defaults are for PDIP-24, inches */
    /// set the case length
    int setCaseLength(double d);
    /// retrieve the case length
    double getCaseLength(void)
    {
        return D;
    }
    /// set the case width
    int setCaseWidth(double e1);
    /// retrieve the case width
    double getCaseWidth(void)
    {
        return E1;
    }
    /// set the case taper
    int setCaseTaper(double s);
    /// retrieve the case taper
    double getCaseTaper(void)
    {
        return S;
    }
    /// set the case base height above the PCB
    int setBaseHeight(double a1);
    /// retrieve the case base height above the PCB
    double getBaseHeight(void)
    {
        return A1;
    }
    /// set the case depth
    int setCaseDepth(double a2);
    /// retrieve the case depth
    double getCaseDepth(void)
    {
        return A2;
    }

    /* ancillary parameters for improved looks */
    /// set the width of the case notch
    int setNotchWidth(double nw);
    /// retrieve the width of the case notch
    double getNotchWidth(void)
    {
        return NW;
    }
    /// set the depth of the case notch
    int setNotchDepth(double nd);
    /// retrieve the depth of the case notch
    double getNotchDepth(void)
    {
        return ND;
    }
    /// set the case bevel value
    int setCaseBevel(double bev);
    /// retrieve the case bevel value
    double getCaseBevel(void)
    {
        return BEV;
    }
    /// set the case center's depth
    int setCaseMidHeight(double mid);
    /// retrieve the case center's depth
    double getCaseMidHeight(void)
    {
        return MID;
    }
    /// set the case notch's length
    int setNotchLength(double nl);
    /// retrieve the case notch's length
    double getNotchLength(void)
    {
        return NL;
    }
    /// set the virtual world's scaling parameter
    void setWorldScale(double sc);
    /// retrieve the virtual world's scaling parameter
    double getWorldScale(void)
    {
        return scale;
    }

    /// calculate the vertices
    Quat *calc(void);
    /// retrieve the total number of points
    int getNP(void)
    {
        return CASE_NP;
    }
    /// write the point list as a VRML Coordinate{}
    int writeCoord(std::ofstream &fp, int tabs = 0, Transform *t = NULL);
    /// write the facet list as a VRML coordIndex[]
    int writeFacets(std::ofstream &fp, int tabs = 0);
};

#endif /* DIP_CASE_H */
