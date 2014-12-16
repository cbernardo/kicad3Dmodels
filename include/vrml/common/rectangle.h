/*
 *      file: rectangle.h
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

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <polygon.h>

namespace KC3D
{
class TRANSFORM;

/**
 * \ingroup vrml_tools
 * This class represents a rectangle; the rectangle
 * may be plain or beveled.
 */
class RECTANGLE : public POLYGON
{
private:
    double bev; ///< the length of the bevel at each corner
    int seg;    ///< number of segments to a bevel

public:
    RECTANGLE();
    RECTANGLE( double aBevel );
    RECTANGLE( const RECTANGLE& aRectangle );
    virtual ~RECTANGLE();

    RECTANGLE& operator=( const RECTANGLE& p );

    /**
     * Create a duplicate instance of this RECTANGLE
     *
     * @return Duplicate of *this
     */
    POLYGON* Clone( void ) const;

    /**
     * \brief Calculate the rectangle's vertices
     *
     * Calculate the vertices representing the rectangle; the rectangle
     * may be plain or beveled. The rectangle is centered on (0,0,0)
     * but may be transformed as the user specifies.
     *
     * @param xLength    [in] length along X axis
     * @param yLength    [in] length along Y axis
     * @param aTransform [in] local transform to apply to results
     * @return 0 for success, -1 for failure
     */
    virtual int Calc( double xLength, double yLength, TRANSFORM& aTransform );

    /**
     * \brief Set the length of the bevel.
     *
     * Set the length of the bevel.
     * A bevel <= 0 results in a plain rectangle (default). The bevel
     * must be less than half the length of the shortest side of
     * the rectangle.
     *
     * @param aBevel [in] length of the bevel; <= 0 means no bevel
     * @param nSegments [in] number of segments (1 = bevel, >1 approximates a radius)
     * @return 0 for success, -1 for failure
     */
    int SetBevel( double aBevel, int nSegments = 1 );
};

}    // namespace KC3D

#endif // RECTANGLE_H
