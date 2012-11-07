/*
 *      file: rectangle.h
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
 *      This class represents a rectangular polygon which can be used to generate
 *      rectangular leads or pins.
 *
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "polygon.h"

namespace kc3d {

class Transform;

/**
 * \ingroup vrml_tools
 * This class represents a rectangle; the rectangle
 * may be plain or beveled.
 */
class Rectangle: public Polygon
{
private:
  double bev;   ///< the length of the bevel at each corner
  
public:
    Rectangle();
    Rectangle(double bevel);
    Rectangle(const Rectangle &p);
    virtual ~Rectangle();

    Rectangle &operator=(const Rectangle &p);

    /**
     * Create a duplicate instance of this Rectangle
     *
     * @return Duplicate of *this
     */
    Polygon *clone(void);

    /**
     * \brief Calculate the rectangle's vertices
     *
     * Calculate the vertices representing the rectangle; the rectangle
     * may be plain or beveled. The rectangle is centered on (0,0,0)
     * but may be transformed as the user specifies.
     *
     * @param xl [in] length along X axis
     * @param yl [in] length along Y axis
     * @param t  [in] local transform to apply to results
     * @return 0 for success, -1 for failure
     */
    virtual int Calc(double xl, double yl, Transform &t);

    /// @param bev   bevel; < 0 means no bevel
    /**
     * \brief Set the length of the bevel.
     *
     * Set the length of the bevel.
     * A bevel <= 0 results in a plain rectangle (default). The bevel
     * must be less than half the length of the shortest side of
     * the rectangle.
     *
     * @param bevel [in] length of the bevel
     */
    void SetBevel(double bevel);
};

}   // namespace kc3d

#endif /* RECTANGLE_H_ */
