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


    // Clone the class
    virtual Polygon *clone(void) = 0;

    // Return values: 0 for success otherwise -1
    // Inscribes a polygon within an ellipse. Use xrad=yrad to obtain
    // regular polygons.
    /// @param xrad Radius along the X axis
    /// @param yrad Radius along the Y axis
    /// @param t Transform to place the polygon in an appropriate orientation in space
    virtual int Calc(double xrad, double yrad, Transform &t) = 0;

    // Append a polygonal face to the Shape section of a VRML file
    // Return values: 0 for success otherwise -1
    /// @param ccw Determines order of points in vertices
    /// @param t Transform to reorient the ellipse
    /// @param color Material appearance to use
    /// @param reuse_color Reuse (true) or define an appearance
    /// @param fp The output stream
    /// @param tabs The tabulator level (max. 4)
    int Paint(bool ccw, Transform &t, VRMLMat &color, bool reuse_color,
            std::ofstream &fp, int tabs = 0);

    // Append a tubular Shape section to a VRML file
    // Return values: 0 for success otherwise -1
    /// @param p2 The polygon to stitch to (must have the same number of vertices)
    /// @param ccw Determines order of points in vertices
    /// @param t Transform to reorient the ellipse
    /// @param color Material appearance to use
    /// @param reuse_color Reuse (true) or define an appearance
    /// @param fp The output stream
    /// @param tabs The tabulator level (max. 4)
    int Stitch(Polygon &p2, bool ccw, Transform &t,
            VRMLMat &color, bool reuse_color, std::ofstream &fp, int tabs = 0);

    // Transform all points in the polygon
    int Xform(Transform &T);

    // Return value: number of points. Handles will point to arrays of doubles
    int GetVertices(double **px, double **py, double **pz);
};



#endif /* POLYGON_H_ */
