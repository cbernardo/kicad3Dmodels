/*
 *      file: wire.h
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

#ifndef WIRE_H_
#define WIRE_H_

#include <iosfwd>
#include <vector>

#include "transform.h"

namespace kc3d
{

class VRMLMat;

/**
 * \ingroup vrml_tools
 * \brief Wire (swept path)
 *
 * This class represents a swept path or wire.
 * A list of points in 3D space is built up to
 * describe the path and a polygon is swept
 * along the path to generate the wire. The
 * algorithms will respect the specified minimum
 * bend radius, but since the algorithm knows
 * nothing of the dimensions of the polygon used,
 * the user is responsible for ensuring that a
 * sensible value is provided. The polygon is
 * assumed to be centered on (0, 0, 0) and it is
 * recommended that the polygon be planar to
 * ensure good results, but the polygon is not
 * inherently restricted to a plane. The wire
 * length is calculated to aid the user in
 * estimating wire lengths.
 */
class Wire
{
private:
    std::vector <kc3d::Quat> path;
    double seg_ang; // segments per PI radians
    double rbend;   // bend radius

    int extrude(Quat &p1, Quat &p2, SimplePoly &shape, double &length,
            bool cap0, bool cap1, bool outside, Transform &t,
            VRMLMat &color, bool &reuse, std::ofstream &file, int tabs = 0);
    int render(Quat &p1, Quat &p2, Quat &p3, Quat &norm, SimplePoly &shape, double &length,
            bool cap0, bool outside, Transform &t, VRMLMat &color, bool reuse,
            std::ofstream &file, int tabs = 0);

public:
    Wire();
    virtual ~Wire();

    int AddPoint(const Quat &q);
    int AddPoint(double x, double y, double z);
    void Clear();

    /**
     * Set parameters used in extruding the wirse
     *
     * @param segments [in] number of segments per PI/2 (90 degree) bend
     * @param bend_radius [in] bend radius (radians)
     * @return 0 for success, -1 for failure
     */
    int SetParams(double segments, double bend_radius);

    double Build(const Polygon &shape, bool cap0, bool cap1, bool outside, Transform &t,
            VRMLMat &color, bool reuse, std::ofstream &file, int tabs = 0);
};

} /* namespace kc3d */
#endif /* WIRE_H_ */
