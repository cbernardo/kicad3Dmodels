/*
 *      file: ring.h
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
 *      The Ring object describes a common element in VRML models of
 *      electronic components. Operations on a ring include the following:
 *          + calculate the vertices using a center point, number of points,
 *              X radius, Y radius, and a Transform.
 *          + render the interior in a forward or reverse vertex order;
 *              this determines which side the ring is visible from.
 *          + stitch the edges of 2 rings to form a tubular section
 *          + retrieve the vertices and number of points
 *
 */

#ifndef RING_H_
#define RING_H_

#include <iosfwd>

#include "material.h"
#include "transform.h"

class Ring;
class Ring
{
private:
    double cx, cy, cz;  // center point after transformation
    double *x, *y, *z;  // array of vertices after transformation
    int np;             // number of vertices (8 .. 360 ; must be multiple of 4)
    bool valid;         // true if Calc has successfully completed

    void init(void);

public:
    Ring();
    virtual ~Ring();

    // Return values: 0 for success otherwise -1
    /// @param np The number of vertices; must be 8..360 inclusive and a multiple of 4
    /// @param xrad Radius along the X axis
    /// @param yrad Radius along the Y axis
    /// @param t Transform to place the ellipse in an appropriate orientation in space
    int Calc(int np, double xrad, double yrad, const Transform &t);

    // Return values: 0 for success otherwise -1
    /// @param name Name for the VRML Transform entity
    /// @param forward Determines order of vertices
    /// @param t Transform to reorient the ellipse
    /// @param color Material appearance to use
    /// @param reuse_color Reuse (true) or define an appearance
    /// @param fp The output stream
    /// @param tabs The tabulator level (max. 4)
    int Render(const std::string &name, bool forward, const Transform &t,
            const VRMLMat &color, bool reuse_color, ofstream &fp, int tabs = 0);

    // Return values: 0 for success otherwise -1
    /// @param name Name for the VRML Transform entity
    /// @param ring The ring to stitch to
    /// @param forward Determines order of vertices
    /// @param t Transform to reorient the ellipse
    /// @param color Material appearance to use
    /// @param reuse_color Reuse (true) or define an appearance
    /// @param fp The output stream
    /// @param tabs The tabulator level (max. 4)
    int Stitch(const std::string &name, const Ring &ring, bool forward, const Transform &t,
            const VRMLMat &color, bool reuse_color, ofstream &fp, int tabs = 0);

    // Return value: number of points. Handles will point to arrays of doubles
    int GetVertices(double **px, double **py, double **pz);

};


#endif /* RING_H_ */
