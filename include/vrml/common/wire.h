/*
 *      file: wire.h
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

#ifndef WIRE_H
#define WIRE_H

#include <iosfwd>
#include <vector>

#include <transform.h>

namespace KC3D
{
class VRMLMAT;

/**
 * \ingroup vrml_tools
 * \brief WIRE (swept path)
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
class WIRE
{
private:
    std::vector <KC3D::QUAT> path;
    double  seg_ang;    // segments per PI radians
    double  rbend;      // bend radius

    int extrude( QUAT& p1, QUAT& p2, SIMPLEPOLY& shape, double& length,
            bool cap0, bool cap1, bool outside, TRANSFORM& t,
            VRMLMAT& color, bool& reuse, std::ofstream& file, int tabs = 0 );

    int render( QUAT& p1, QUAT& p2, QUAT& p3, QUAT& norm, SIMPLEPOLY& shape, double& length,
            bool cap0, bool outside, TRANSFORM& t, VRMLMAT& color, bool reuse,
            std::ofstream& file, int tabs = 0 );

public:
    WIRE();
    virtual ~WIRE();

    int     AddPoint( const QUAT& aPoint );
    int     AddPoint( double x, double y, double z );
    void    Clear();

    /**
     * Set parameters used in extruding the wirse
     *
     * @param segments [in] number of segments per PI/2 (90 degree) bend
     * @param bend_radius [in] bend radius (radians)
     * @return 0 for success, -1 for failure
     */
    int SetParams( double aNSegments, double aBendRadius );

    double Build( const POLYGON& aShape, bool aRenderCap0, bool aRenderCap1,
                  bool isOutside, TRANSFORM& aTransform, VRMLMAT& aMaterial,
                  bool reuseMaterial, std::ofstream& aVRMLFile, int aTabDepth = 0 );
};

}    // namespace KC3D
#endif // WIRE_H
