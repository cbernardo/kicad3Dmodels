/*
 *      file: polyrect.h
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

#ifndef POLYRECT_H_
#define POLYRECT_H_

#include "polygon.h"

class PolyRect: public Polygon
{
public:
    PolyRect();
    PolyRect(const PolyRect &p);
    virtual ~PolyRect();

    PolyRect &operator=(const PolyRect &p);

    // Return values: 0 for success otherwise -1
    // Inscribes a polygon within an ellipse. Use xrad=yrad to obtain
    // regular polygons.
    /// @param xl length along the X axis
    /// @param yl length along the Y axis
    /// @param t Transform to place the polygon in an appropriate orientation in space
    /// @bev   bevel; < 0 means no bevel
    int Calc(double xl, double yl, Transform &t, double bev = -1.0);

};

#endif /* POLYRECT_H_ */
