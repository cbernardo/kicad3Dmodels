/*
 *      file: circle.h
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

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include <iosfwd>

#include "polygon.h"
class Transform;
class VRMLMat;

class Circle : public Polygon
{
public:
    Circle();
    Circle(int vertices);
    Circle(const Circle &p);
    virtual ~Circle();

    Circle &operator=(const Circle &p);

    // Clone the object
    Polygon *clone(void);

    // Return values: 0 for success otherwise -1
    // Inscribes a polygon within an ellipse. Use xrad=yrad to obtain
    // regular polygons.
    /// @param np The number of vertices; must be 3..360 inclusive
    /// @param xdia Diameter along the X axis
    /// @param ydia Diameter along the Y axis
    /// @param t Transform to place the polygon in an appropriate orientation in space
    virtual int Calc(double xdia, double ydia, Transform &t);

    int SetNVertices(int nvert);
};



#endif /* CIRCLE_H_ */
