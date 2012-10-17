/*
 *      file: point.cpp
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
 */


#include "point.h"

Point::Point()
{
    x = y = z = 0.0f;
    return;
}

Point::Point(double px, double py, double pz)
{
    x = px;
    y = py;
    z = pz;
    return;

}

Point Point::operator-()
{
    Point local(-x, -y, -z);
    return local;
}

Point Point::operator+(Point arg)
{
    Point local(arg.x + x, arg.y + y, arg.z + z);
    return local;
}

Point Point::operator-(Point arg)
{
    Point local(x -arg.x, y -arg.y, z -arg.z);
    return local;
}

Point Point::operator*(double arg)
{
    Point local(x*arg, y*arg, z*arg);
    return local;
}

