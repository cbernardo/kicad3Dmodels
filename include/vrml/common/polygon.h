/*
 *      file: polygon.h
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
 *      This class represents a regular polygon. Override the default
 *      Paint() routine in any derived class which is not a regular polygon.
 *
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include <iosfwd>

#include "vrmlmat.h"
#include "transform.h"

namespace kc3d {

/**
 * \ingroup vrml_tools
 * This class represents a generic polygon and has
 * procedures implemented for rendering convex polygons.
 */
class Polygon
{
private:
    Polygon &operator=(const Polygon &p);

protected:
    double *x, *y, *z;  // array of vertices after transformation
    int nv;             // number of vertices (3 .. 360)
    bool valid;         // true if Calc has successfully completed

    virtual void init(void);

public:
    Polygon();
    virtual ~Polygon();

    /**
     * Test if the object holds a valid set of vertices
     *
     * @return TRUE if a valid vertex set is present
     */
    bool IsValid(void);

    /**
     * \brief Create a duplicate instance of this type of Polygon
     *
     * This method must be implemented by all derived classes to
     * provide a means of creating a copy of the derived class.
     *
     * @return Duplicate instance of *this
     */
    virtual Polygon *clone(void) = 0;

    /**
     * \brief Calculate the vertices of a polygon
     *
     * Each derived class must implement this method to calculate
     * a set of vertices representing the derived class.
     *
     * @param x [in] maximum extent of the polygon along the X axis
     * @param y [in] maximum extent of the polygon along the Y axis
     * @param t [in] local transform to apply to results
     * @return 0 for success, -1 for failure
     */
    virtual int Calc(double x, double y, Transform &t) = 0;

    /**
     * \brief Render the face of a polygon
     *
     * Write out a VRML2.0 Shape block defining a rendered face of
     * a convex polygon. A derived class must override this method
     * if it implements a concave polygon.
     *
     * @param ccw   [in] order of vertex enumeration; this determines the side from which a polygon is visible
     * @param t     [in] geometric transform to apply to output vertices
     * @param color [in] VRMLMat material appearance specification
     * @param reuse_color [in] set to TRUE to reuse a previously written material name
     * @param fp    [in] open output file
     * @param tabs  [in] indent level for formatting
     * @return 0 for success, -1 for failure
     */
    int Paint(bool ccw, Transform &t, VRMLMat &color, bool reuse_color,
            std::ofstream &fp, int tabs = 0);

    /**
     * \brief Render a surface defined by the perimeter of 2 polygons
     *
     * Write out a series of VRML2.0 Shape blocks defining a surface
     * described by the vertices of two polygons.  The polygons must
     * have the same number of vertices. As an example of its use,
     * two circles can be used to create a tubular or cylindrical
     * section as well as a disc with a hole in it.
     *
     * @param p2    [in] the second polygon describing the edge of the surface
     * @param ccw   [in] order of vertex enumeration; this determines the side from which a facet is visible
     * @param t     [in] geometric transform to apply to output vertices
     * @param color [in] VRMLMat material appearance
     * @param reuse_color [in] TRUE to reuse a previously written material name
     * @param fp    [in] open output file
     * @param tabs  [in] indent level for formatting
     * @return
     */
    int Stitch(Polygon &p2, bool ccw, Transform &t,
            VRMLMat &color, bool reuse_color, std::ofstream &fp, int tabs = 0);

    /**
     * \brief Transform vertices
     *
     * Transform all the vertices in a polygon. Unlike the transform parameters
     * supplied to the routines Paint and Stitch which only affects the output,
     * this transform alters the object's internally stored vertices.
     *
     * @param T [in] geometric transform to apply to internal vertex list
     * @return 0 for success, -1 for failure
     */
    int Xform(Transform &T);

    /**
     * \brief Retrieve the internal vertex list
     * @param px [out] pointer to internal array of X coordinates
     * @param py [out] pointer to internal array of Y coordinates
     * @param pz [out] pointer to internal array of Z coordinates
     * @return number of vertices in the list or -1 for failure
     */
    int GetVertices(double **px, double **py, double **pz);
};

}   // namespace kc3d

#endif /* POLYGON_H_ */
