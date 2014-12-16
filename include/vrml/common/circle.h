/*
 *      file: circle.h
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
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include <iosfwd>

#include <polygon.h>

namespace KC3D
{
class TRANSFORM;
class VRMLMAT;

/**
 * \ingroup vrml_tools
 * This class represents an ellipse, or more precisely
 * an inscribed regular convex polygon of 3 to 360 vertices
 * which may be independently scaled along the X and Y axes.
 */
class CIRCLE : public POLYGON
{
public:
    CIRCLE();
    CIRCLE( int nVertices );
    CIRCLE( const CIRCLE& aCircle );
    virtual ~CIRCLE();

    CIRCLE& operator=( const CIRCLE& aCircle );

    /**
     * Create a duplicate instance of this CIRCLE.
     * Caveat: if system resources are low, the copy may
     * not be a valid duplicate.
     *
     * @return Duplicate of *this
     */
    POLYGON* Clone( void ) const;

    /**
     * \brief Calculate the vertices of an ellipse
     *
     * This method calculates the vertices representing an ellipse.
     * By default the ellipse will contain 16 vertices; call
     * SetNVertices prior to invoking this method to set a different
     * number of vertices.  The ellipse is centered on (0,0,0)
     * but may be transformed as the user specifies.
     *
     * This method may be abused to create general polygons
     * inscribed within an ellipse; the first vertex will
     * always be at the coordinate (xdia, 0, 0) and subsequent
     * vertices will be created in a counter-clockwise direction
     * when viewing the XY plane from a point at +Z.
     *
     * @param xDia [in] diameter of the ellipse along the X axis
     * @param yDia [in] diameter of the ellipse along the Y axis
     * @param aTransform [in] local transformation to apply to the results
     * @return 0 for success, -1 for failure
     */
    virtual int Calc( double xDia, double yDia, TRANSFORM& aTransform );

    /**
     * Set the number of vertices used to represent an ellipse
     *
     * @param nvert [in] number of vertices; valid range is 3 .. 360
     * @return 0 for success, -1 for failure
     */
    int SetNVertices( int nVert );
};

}    // namespace kc3d

#endif /* CIRCLE_H_ */
