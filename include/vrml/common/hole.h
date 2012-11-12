/*
 *      file: hole.h
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
 *      This object represents a rectangular frame with a hole in it.
 *      The hole may be rectangular or elliptical and may be offset.
 */

#ifndef HOLE_H_
#define HOLE_H_

// XXX -
// TODO: support a beveled inner hole

#include <iosfwd>

namespace kc3d {

class Polygon;
class Transform;
class VRMLMat;

/**
 * \ingroup vrml_tools
 * This class represents a rectangular face with a
 * rectangular or elliptical hole within its boundaries.
 */
class Hole
{
private:
    bool valid;     ///< TRUE if there is a valid set of vertices
    bool square;    ///< TRUE if the hole is rectangular, FALSE for an elliptical hole
    double w1;      ///< width (X axis) of frame
    double w2;      ///< width (X axis) of hole
    double d1;      ///< depth (Y axis) of frame
    double d2;      ///< depth (Y axis) of hole
    double ow;      ///< offset of hole along X axis (default 0 = centered)
    double od;      ///< offset of hole along Y axis (default 0 = centered)
    Polygon *hole;  ///< polygons representing the hole (circular or rectangular)
    double v[3][4]; ///< vertices for the rectangular frame
    int np;         ///< number of points in a circular hole

    /**
     * Write the facet index list
     *
     * @param top   [in] TRUE if the surface is to be visible from the top (+Z)
     * @param fp    [in] open output file
     * @param tabs  [in] indent level for formatting
     * @return
     */
    int writeRFacets(std::ofstream &fp, int tabs = 0);

    /**
     * Write the facet coordinate list
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
    int writeFacets(int v0, int v1, int h0, int h1, int lp,
            std::ofstream &fp, int tabs = 0, bool term = false);

    // Hide the default copy and assignment operators
    Hole(const Hole &p);
    Hole &operator=(const Hole &p);

public:
    Hole();
    virtual ~Hole();

    /**
     * Calculate vertices for a rectangular surface with a hole
     *
     * @param w1 [in] width (X axis) of the frame
     * @param d1 [in] depth (Y axis) of the frame
     * @param w2 [in] width (X axis) of the hole
     * @param d2 [in] depth (Y axis) of the hole
     * @param tx [in] geometric transformation to apply to results
     * @param square [in] TRUE for a rectangular hole; FALSE for an elliptical hole
     * @param ow [in] hole offset along X axis (default 0 = centered on X)
     * @param od [in] hole offset along Y axis (default 0 = centered on Y)
     * @param np [in] number of vertices for an elliptical hole (must be multiple of 4 in the range 4..360)
     * @return 0 for success, -1 for failure
     */
    int Calc(double w1, double d1, double w2, double d2, Transform &tx,
            bool square, double ow = 0.0, double od = 0.0, int np = 16);

    /**
     * Render a surface with a hole
     *
     * @param top [in] TRUE if visible from +Z
     * @param t   [in] geometric transform to apply to output vertices
     * @param color [in] VRMLMat material appearance
     * @param reuse [in] TRUE to reuse a previously written material name
     * @param fp    [in] open output file
     * @param tabs  [in] indent level for formatting
     * @return 0 for success, -1 for failure
     */
    int Build(Transform &t, VRMLMat &color, bool reuse,
            std::ofstream &fp, int tabs = 0);

};

}   // namespace kc3d

#endif /* HOLE_H_ */
