/*
 *      file: funnel.h
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
 *      This class represents a rectangular or circular funnel which can
 *      be used to create recessed pins.
 *
 *             _______
 *            /      / = d1
 *           /__w1__/
 *           \     /  = h1
 *            \___/
 *            |   | | = h2
 *            |___|/|
 *            |   | |
 *            |   | | = h3
 *            |   | |
 *            |_w2|/  = d2
 *
 *            w1, d1 = x, y dimensions of top of flute
 *            w2, d2 = x, y dimensions of stem
 *            h1 = height of flute
 *            h2 = height of stem which will be FluteColor
 *            h3 = height of stem which will be StemColor
 *            FluteColor = color of the flute
 *            StemColor  = color of the stem
 *            (bool) cap = true if the stem is to be capped at the bottom
 *
 *      Sample parameters:
 *          w1 = 1.8;
 *          d1 = 1.8;
 *          w2 = 0.9;
 *          d2 = 0.9;
 *          h1 = 0.7;
 *          h2 = 0.7;
 *          h3 = 4.0;
 *
 */

#ifndef FUNNEL_H
#define FUNNEL_H

#include <iosfwd>

#include <polygon.h>

namespace KC3D
{
class VRMLMAT;
class TRANSFORM;

/**
 * \ingroup vrml_tools
 * This class represents a funnel.  A funnel consists of
 * a flute and a stem which are only visible from the inside.
 * A funnel may be used to render a receptacle. The shape may
 * be rectangular or elliptical in cross-section.
 */
class FUNNEL
{
private:
    bool    valid;  ///< TRUE if there is a valid vertex set
    bool    square; ///< TRUE if the funnel is rectangular (default)
    int     npoly;  ///< number of polygons in the funnel
    double bev;     ///< bevel (for square funnels only)
    POLYGON** poly; ///< list of polygons representing the funnel

    void cleanup( void );

public:
    FUNNEL();
    FUNNEL( const FUNNEL& p );
    virtual ~FUNNEL();

    FUNNEL& operator=( const FUNNEL& p );

    /***
     * \brief Calculate the polygons defining a funnel
     *
     * Calculate the polygons which bound a funnel.  The default
     * funnel shape has a rectangular cross-section but an elliptical
     * cross-section may be chosen by invoking SetShape prior to
     * invoking this method.
     *
     * @param aFluteXWidth [in] width (X) of the flute
     * @param aFluteYDepth [in] depth (Y) of the flute
     * @param aStemXWidth [in] width (X) of the stem
     * @param aStemYDepth [in] width (Y) of the stem
     * @param aFluteLength [in] length (Z) of the flute; must be >= 0
     * @param aStemLength [in] length (Z) of stem which is same color as flute (<= 0 == none)
     * @param aStemLength2 [in] length (Z) of stem different in color to flute (must be > 0)
     * @param aTransform [in] geometric transform to apply to results
     * @param aNumberSides [in] optional number of vertices in elliptical flute cross-section
     * @return 0 for success, -1 for failure
     */
    int Calc( double aFluteXWidth, double aFluteYDepth, double aStemXWidth,
              double aStemYDepth, double aFluteLength, double aStemLength,
              double aStemLength2, TRANSFORM& aTransform, int aNumberSides = 16 );

    /**
     * \brief Render a funnel
     *
     * Write a series of VRML2.0 Shape blocks to define a funnel
     *
     * @param cap [in] TRUE if the bottom face of the stem is to be rendered
     * @param t   [in] geometric transform to apply to output vertices
     * @param flutecolor [in] VRMLMAT material appearance for the flute
     * @param reuse_flute [in] TRUE if a previously written definition of 'flutecolor' is to be used
     * @param stemcolor [in] VRMLMAT material appearance for the stem
     * @param reuse_stem [in] TRUE if a previously written definition of 'stemcolor' is to be used
     * @param fp [in] open output file
     * @param tabs [in] indent level for formatting
     * @return 0 for success, -1 for failure
     */
    int Build( bool aRenderCap, TRANSFORM& aTransform, VRMLMAT& aFluteMat, bool reuseFluteMat,
               VRMLMAT& aStemMat, bool reuseStemMat, std::ofstream& aVRMLFile,
               int aTabDepth = 0 );

    /**
     * Set the cross-section shape of the funnel
     *
     * @param isSquare [in] TRUE for a rectangular cross-section, FALSE for an elliptical cross-section
     * @param aBevel [in] >0 if a rectangular flute is to be beveled; bev must be < 1/2 the smallest cross-sectional dimension of the stem
     */
    void SetShape( bool isSquare, double aBevel = -1.0 );
};

}    // namespace KC3D

#endif // FUNNEL_H
