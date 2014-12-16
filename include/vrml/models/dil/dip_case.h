/*
 *  file: dip_case.h
 *
 *  Copyright 2012-2014 Cirilo Bernardo (cjh.bernardo@gmail.com)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 *  Notes:
 *  These classes were created to support the
 *  programmatic creation of VRML models for
 *  use with KiCad.
 *
 */

#ifndef DIP_CASE_H
#define DIP_CASE_H

#include <iosfwd>

#include <transform.h>

// note: original CASE_NP was 76; it was increased
// to 80 to support an internal rectangle to improve
// the cosmetic appearance. Without the internal
// facet, you can see pinholes through the top
// edge of the case.
#define CASE_NP (80)

namespace KC3DDIP
{

class DIPCASE
{
private:
    KC3D::QUAT p[CASE_NP];
    double  NW;     ///< [NotchWidth]
    double  ND;     ///< [NotchDepth]
    double  BEV;    ///< [CaseBevel]
    double  MID;    ///< [CaseMidHeight] depth of middle portion where pins attach
    double  NL;     ///< [NotchLength] must be > NW/2
    double  D;      ///< [CaseLength]
    double  E1;     ///< [CaseWidth]
    double  S;      ///< [CaseTaper] deviation of unbeveled top and bottom edges
    double  A1;     ///< [BaseHeight] distance from board to bottom of case
    double  A2;     ///< [CaseDepth]
    bool    valid;  ///< true when the current set of vertices are valid

public:
    DIPCASE();

    /**
     * Set the case parameters
     *
     * @param aCaseLength [in] case length
     * @param aCaseWidth [in] case width
     * @param aPCBOffset [in] distance from top of PCB to bottom of case
     * @param aCaseDepth [in] depth of the case
     * @param aNotchWidth [in] notch width
     * @param aNotchDepth [in] notch depth (must be < (a1-mid)/2)
     * @param aNotchLength [in] notch length
     * @param aCaseMidZ [in] depth of middle untapered section
     * @param aBevel [in] bevel on case corners
     * @param aTaper [in] an inward offset of case top and bottom
     * @return 0 for success, -1 for failure
     */
    int SetParams( double aCaseLength, double aCaseWidth, double aPCBOffset,
                   double aCaseDepth, double aNotchWidth, double aNotchDepth,
                   double aNotchLength, double aCaseMidZ, double aBevel, double aTaper );

    /// set the case length
    int SetCaseLength( double aCaseLength );

    /// retrieve the case length
    double GetCaseLength( void )
    {
        return D;
    }

    /// set the case width
    int SetCaseWidth( double aCaseWidth );

    /// retrieve the case width
    double GetCaseWidth( void )
    {
        return E1;
    }

    /// set the case taper
    int SetCaseTaper( double aTaper );

    /// retrieve the case taper
    double GetCaseTaper( void )
    {
        return S;
    }

    /// set the case base height above the PCB
    int SetBaseHeight( double aPCBOffset );

    /// retrieve the case base height above the PCB
    double GetBaseHeight( void )
    {
        return A1;
    }

    /// set the case depth
    int SetCaseDepth( double aCaseDepth );

    /// retrieve the case depth
    double GetCaseDepth( void )
    {
        return A2;
    }

    /* ancillary parameters for improved looks */
    /// set the width of the case notch
    int SetNotchWidth( double aNotchWidth );

    /// retrieve the width of the case notch
    double GetNotchWidth( void )
    {
        return NW;
    }

    /// set the depth of the case notch
    int SetNotchDepth( double aNotchDepth );

    /// retrieve the depth of the case notch
    double GetNotchDepth( void )
    {
        return ND;
    }

    /// set the case bevel value
    int SetCaseBevel( double aBevel );

    /// retrieve the case bevel value
    double GetCaseBevel( void )
    {
        return BEV;
    }

    /// set the case center's depth
    int SetCaseMidHeight( double aCaseMidZ );

    /// retrieve the case center's depth
    double GetCaseMidHeight( void )
    {
        return MID;
    }

    /// set the case notch's length
    int SetNotchLength( double aNotchLength );

    /// retrieve the case notch's length
    double GetNotchLength( void )
    {
        return NL;
    }

    /// calculate the vertices
    int Calc( void );

    /// write the point list as a VRML Coordinate{}
    int WriteCoord( KC3D::TRANSFORM& aTransform, std::ofstream& aVRMLFile, int aTabDepth = 0 );

    /// write the facet list as a VRML coordIndex[]
    int WriteFacets( std::ofstream& aVRMLFile, int aTabDepth = 0 );
};

}    // namespace KC3DDIP

#endif // DIP_CASE_H
