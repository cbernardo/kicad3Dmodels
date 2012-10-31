/*
 *      file: shoulder.h
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
 *      This class represents a shoulder such as the one found on the
 *      bottom of a SIL header which creates a small gap between the
 *      component and the board.
 *
 *      _______o_______ l = length
 *      \             / h = height
 *       \___________/  w = width
 *                      t = taper (radians)
 *                      r = radius of inner lip
 */

#ifndef SHOULDER_H_
#define SHOULDER_H_

class Transform;
class VRMLMat;

class Shoulder
{
private:
    bool valid;
    bool square;
    double p0[3][7], p1[3][7];  // the 2 polygons defining the shoulder
    int np; // number of points defining the shoulder

public:
    Shoulder();
    virtual ~Shoulder();

    int Calc(double l, double h, double w, double t, double r, Transform &tx);

    int Build(Transform &t, VRMLMat &color, bool reuse,
            std::ofstream &fp, int tabs = 0);
};

#endif /* SHOULDER_H_ */
