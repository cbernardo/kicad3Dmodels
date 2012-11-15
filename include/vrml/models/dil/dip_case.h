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

namespace kc3ddip {

class DipCase
{
private:
    kc3d::Quat p[CASE_NP];
    double NW;      ///< [NotchWidth]
    double ND;      ///< [NotchDepth]
    double BEV;     ///< [CaseBevel]
    double MID;     ///< [CaseMidHeight] depth of middle portion where pins attach
    double NL;      ///< [NotchLength] must be > NW/2
    double D;       ///< [CaseLength]
    double E1;      ///< [CaseWidth]
    double S;       ///< [CaseTaper] deviation of unbeveled top and bottom edges
    double A1;      ///< [BaseHeight] distance from board to bottom of case
    double A2;      ///< [CaseDepth]
    bool valid;     ///< true when the current set of vertices are valid

public:
    DipCase();

    /**
     * Set the case parameters
     *
     * @param d [in] case length
     * @param e1 [in] case width
     * @param a1 [in] distance from top of PCB to bottom of case
     * @param a2 [in] depth of the case
     * @param nw [in] notch width
     * @param nd [in] notch depth (must be < (a1-mid)/2)
     * @param nl [in] notch length
     * @param mid [in] depth of middle untapered section
     * @param bev [in] bevel
     * @param s [in] inward offset of case top and bottom
     * @return 0 for success, -1 for failure
     */
    int setParams(double d, double e1, double a1, double a2,
            double nw, double nd, double nl, double mid, double bev, double s);

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

    /// calculate the vertices
    int calc(void);

    /// write the point list as a VRML Coordinate{}
    int writeCoord(kc3d::Transform &t, std::ofstream &fp, int tabs = 0);
    /// write the facet list as a VRML coordIndex[]
    int writeFacets(std::ofstream &fp, int tabs = 0);
};

}   // namespace kc3ddip

#endif /* DIP_CASE_H */
