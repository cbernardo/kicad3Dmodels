/*
 *      file: hole.h
 *
 *      Copyright 2012-2014 Dr. Cirilo Bernardo (cjh.bernardo@gmail.com)
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

#ifndef HOLE_H
#define HOLE_H

#include <iosfwd>

namespace KC3D
{
class POLYGON;
class TRANSFORM;
class VRMLMAT;

/**
 * \ingroup vrml_tools
 * This class represents a rectangular face with a
 * rectangular or elliptical hole within its boundaries.
 */
class HOLE
{
private:
    bool    valid;          ///< TRUE if there is a valid set of vertices
    bool    square;         ///< TRUE if the hole is rectangular, FALSE for an elliptical hole
    double  w1;             ///< width (X axis) of frame
    double  w2;             ///< width (X axis) of hole
    double  d1;             ///< depth (Y axis) of frame
    double  d2;             ///< depth (Y axis) of hole
    double  bev;            ///< bevel on inner hole (rectangular only)
    double  ow;             ///< offset of hole along X axis (default 0 = centered)
    double  od;             ///< offset of hole along Y axis (default 0 = centered)
    POLYGON*    hole;       ///< polygons representing the hole (circular or rectangular)
    double      v[3][4];    ///< vertices for the rectangular frame
    int np;                 ///< number of points in a circular hole

    /**
     * Write the facet index list for a plain rectangular hole
     *
     * @param top   [in] TRUE if the surface is to be visible from the top (+Z)
     * @param fp    [in] open output file
     * @param tabs  [in] indent level for formatting
     * @return
     */
    int writeRFacets( bool top, std::ofstream& fp, int tabs = 0 );

    /**
     * Write the facet index list for a beveled rectangular hole
     *
     * @param top   [in] TRUE if the surface is to be visible from the top (+Z)
     * @param fp    [in] open output file
     * @param tabs  [in] indent level for formatting
     * @return
     */
    int writeRFacetsB( bool top, std::ofstream& fp, int tabs = 0 );

    /**
     * Write the facet index list for a circular hole
     *
     * @param v0 [in] starting index for frame vertices
     * @param v1 [in] ending index for frame vertices
     * @param h0 [in] starting index for hole vertices
     * @param h1 [in] ending index for hole vertices
     * @param lp [in] index for final hole vertex
     * @param top [in] TRUE if visible from +Z
     * @param fp  [in] open output file
     * @param tabs [in] indent level for formatting
     * @return
     */
    int writeFacets( bool top, int v0, int v1, int h0, int h1, int lp,
            std::ofstream& fp, int tabs = 0, bool term = false );

    // Hide the default copy and assignment operators
    HOLE( const HOLE& p );
    HOLE& operator=( const HOLE& p );

public:
    HOLE();
    virtual ~HOLE();

    /**
     * Calculate vertices for a rectangular surface with a hole
     *
     * @param aPanelWidth [in] width (X axis) of the frame
     * @param aPanelDepth [in] depth (Y axis) of the frame
     * @param aHoleWidth [in] width (X axis) of the hole
     * @param aHoleDepth [in] depth (Y axis) of the hole
     * @param aTransform [in] geometric transformation to apply to results
     * @param isSquare [in] TRUE for a rectangular hole; FALSE for an elliptical hole
     * @param aHoleWOff [in] hole offset along X axis (default 0 = centered on X)
     * @param aHoleDOff [in] hole offset along Y axis (default 0 = centered on Y)
     * @param aNVertex [in] number of vertices in elliptical hole (multiple of 4, range 4..360)
     * @param aBevel [in] bevel on the inner (rectangular) hole
     * @return 0 for success, -1 for failure
     */
    int Calc( double aPanelWidth, double aPanelDepth, double aHoleWidth, double aHoleDepth,
              TRANSFORM& aTransform, bool isSquare, double aHoleWOff = 0.0,
              double aHoleDOff = 0.0, int aNVertex = 16, double aBevel = -1.0 );

    /**
     * Render a surface with a hole
     *
     * @param isTopView [in] TRUE if visible from +Z
     * @param aTransform [in] geometric transform to apply to output vertices
     * @param aMaterial [in] VRMLMAT material appearance
     * @param reuseMaterial [in] TRUE to reuse a previously written material name
     * @param aVRMLFile [in] open output file
     * @param aTabDepth [in] indent level for formatting
     * @return 0 for success, -1 for failure
     */
    int Build( bool isTopView, TRANSFORM& aTransform, VRMLMAT& aMaterial, bool reuseMaterial,
               std::ofstream& aVRMLFile, int aTabDepth = 0 );
};

}    // namespace KC3D

#endif // HOLE_H
