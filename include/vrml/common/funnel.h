/*
 *      file: funnel.h
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

#ifndef FUNNEL_H_
#define FUNNEL_H_

#include <iosfwd>
#include "polygon.h"

namespace kc3d {

class VRMLMat;
class Transform;

/**
 * \ingroup vrml_tools
 * This class represents a funnel.  A funnel consists of
 * a flute and a stem which are only visible from the inside.
 * A funnel may be used to render a receptacle. The shape may
 * be rectangular or elliptical in cross-section.
 */
class Funnel
{
private:
    bool   valid;   ///< TRUE if there is a valid vertex set
    bool   square;  ///< TRUE if the funnel is rectangular (default)
    int    npoly;   ///< number of polygons in the funnel
    double bev;     ///< bevel (for square funnels only)
    Polygon **poly; ///< list of polygons representing the funnel

    void cleanup(void);


public:
    Funnel();
    Funnel (const Funnel &p);
    virtual ~Funnel();

    Funnel &operator=(const Funnel &p);

    /***
     * \brief Calculate the polygons defining a funnel
     *
     * Calculate the polygons which bound a funnel.  The default
     * funnel shape has a rectangular cross-section but an elliptical
     * cross-section may be chosen by invoking SetShape prior to
     * invoking this method.
     *
     * @param w1 [in] width (X axis) of the flute
     * @param d1 [in] depth (Y axis) of the flute
     * @param w2 [in] width (X axis) of the stem
     * @param d2 [in] width (Y axis) of the stem
     * @param h1 [in] length (Z axis) of the flute; must be >= 0
     * @param h2 [in] length (Z axis) of the stem portion which is the same color as the flute (<= 0 means none)
     * @param h3 [in] length (Z axis) of the stem portion different in color to the flute (must be > 0)
     * @param t  [in] geometric transform to apply to results
     * @param ns [in] (optional) number of vertices in an elliptical flute cross-section
     * @return 0 for success, -1 for failure
     */
    int Calc(double w1, double d1, double w2, double d2,
            double h1, double h2, double h3, Transform &t, int ns = 16);

    /**
     * \brief Render a funnel
     *
     * Write a series of VRML2.0 Shape blocks to define a funnel
     *
     * @param cap [in] TRUE if the bottom face of the stem is to be rendered
     * @param t   [in] geometric transform to apply to output vertices
     * @param flutecolor [in] VRMLMat material appearance for the flute
     * @param reuse_flute [in] TRUE if a previously written definition of 'flutecolor' is to be used
     * @param stemcolor [in] VRMLMat material appearance for the stem
     * @param reuse_stem [in] TRUE if a previously written definition of 'stemcolor' is to be used
     * @param fp [in] open output file
     * @param tabs [in] indent level for formatting
     * @return 0 for success, -1 for failure
     */
    int Build(bool cap, Transform &t, VRMLMat &flutecolor, bool reuse_flute,
            VRMLMat &stemcolor, bool reuse_stem, std::ofstream &fp, int tabs = 0);

    /**
     * Set the cross-section shape of the funnel
     *
     * @param square [in] TRUE for a rectangular cross-section, FALSE for an elliptical cross-section
     * @param bev [in] >0 if a rectangular flute is to be beveled; bev must be < 1/2 the smallest cross-sectional dimension of the stem
     */
    void SetShape(bool square, double bev = -1.0);
};

}   // namespace kc3d

#endif /* FUNNEL_H_ */
