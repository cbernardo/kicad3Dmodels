/*
 *      file: point.h
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
 *      The Point class is a simple container for a 3D point (X, Y, Z)
 */

#ifndef POINT_H_
#define POINT_H_

class Point
{
public:
    double x, y, z;

    Point();
    Point(double px, double py, double pz);

    virtual ~Point();
    Point operator-();
    Point operator+(Point arg);
    Point operator-(Point arg);
    Point operator*(double arg);
};


#endif /* POINT_H_ */
