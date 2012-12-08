/*
 *      file: wire.cpp
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

#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>

#include "vdefs.h"
#include "vrmlmat.h"
#include "transform.h"
#include "vcom.h"
#include "polygon.h"
#include "wire.h"

using namespace std;


namespace kc3d
{

Wire::Wire()
{
    seg_ang = 10.0/M_PI;    // 5 segments per 90 degree bend
    rbend = 1.0;            // 1 unit bend radius
    return;
}

Wire::~Wire()
{
    path.clear();
    return;
}


int Wire::AddPoint(const Quat &q)
{
    Quat tmp = q;
    tmp.w = 0;
    try
    {
        path.push_back(tmp);
    }
    catch (...)
    {
        ERRBLURB;
        cerr << "could not add point to list\n";
        return -1;
    }
    return 0;
}

int Wire::AddPoint(double x, double y, double z)
{
    Quat tmp(0, x, y, z);
    try
    {
        path.push_back(tmp);
    }
    catch (...)
    {
        ERRBLURB;
        cerr << "could not add point to list\n";
        return -1;
    }
    return 0;
}

void Wire::Clear()
{
    path.clear();
    return;
}

int Wire::SetParams(double segments, double bend_radius)
{
    if ((segments < 2) || (segments > 90))
    {
        ERRBLURB;
        cerr << "invalid segments per 90 degrees; valid values are 2 to 90\n";
        return -1;
    }
    if (bend_radius < 1e-9)
    {
        ERRBLURB;
        cerr << "invalid bend radius; must be > 0\n";
        return -1;
    }
    seg_ang = 2.0*segments/M_PI;
    rbend = bend_radius;
    return 0;
}

double Wire::Build(const Polygon &shape, bool cap0, bool cap1,
        bool outside, Transform &t, VRMLMat &color, bool reuse,
        std::ofstream &file, int tabs)
{
    vector<kc3d::Quat>::const_iterator idx = path.begin();
    vector<kc3d::Quat>::const_iterator end = path.end();

    if (!shape.IsValid())
    {
        ERRBLURB;
        cerr << "invalid shape\n";
        return -1;
    }
    SimplePoly outline(shape);
    if (!outline.IsValid())
    {
        ERRBLURB;
        cerr << "could not create polygon\n";
        return -1;
    }

    double length = 0.0;

    int acc = 0;
    bool freuse = reuse;
    Quat p1, p2, p3;
    p1 = *idx++;
    p2 = *idx++;

    // calculate the initial orientation of the shape
    Quat v0(0, 0, 0, 1);
    Quat v1;
    v1 = p2 - p1;
    v0 = v0.cross(v1);
    v0.vnormalize();
    if ((v0.x*v0.x + v0.y*v0.y + v0.z*v0.z) < 1e-9)
    {
        if (v0.w > 0.0)
        {
            // antiparallel
            v0.x = 0.0;
            v0.y = 1;
            v0.z = 0.0;
        }
        else
        {
            // parallel, no change
            v0.w = 0.0;
            v0.x = 0.0;
            v0.y = 0.0;
            v0.z = 1.0;
        }
    }
    Transform t0;
    t0.setTranslation(p1);
    t0.setRotation(v0);
    outline.Xform(t0);

    if (idx == end)
    {
        acc += extrude(p1, p2, outline, length, cap0, cap1, outside, t, color, freuse, file, tabs);
        if (acc)
        {
            ERRBLURB;
            cerr << "problems writing extrusion to file\n";
            return -1;
        }
        return 0;
    }

    do {
        p3 = *idx++;
        acc += render(p1, p2, p3, v0, outline, length, cap0, outside, t, color, freuse, file, tabs);
        freuse = true;
        cap0 = false;
    } while ((idx != end) && (acc == 0));
    if (acc == 0)
    {
        acc += extrude(p1, p2, outline, length, false, cap1, outside, t, color, freuse, file, tabs);
    }

    if (acc)
    {
        ERRBLURB;
        cerr << "problems writing extrusion to file\n";
        return -1;
    }

    return length;
} // Build()





int Wire::extrude(Quat &p1, Quat &p2, SimplePoly &shape, double &length,
        bool cap0, bool cap1, bool outside, Transform &t,
        VRMLMat &color, bool &reuse, std::ofstream &file, int tabs)
{
    if (!shape.IsValid())
    {
        ERRBLURB;
        cerr << "invalid polygon (!IsValid())\n";
        return -1;
    }

    int acc = 0;
    Quat v1;
    Transform t0;

    v1 = p2 - p1;
    t0.setTranslation(v1);
    if (!reuse)
    {
        reuse = true;
        acc += shape.Extrude(cap0, cap1, outside, p1, t0, t, color, false, file, tabs);
    }
    else
    {
        acc += shape.Extrude(cap0, cap1, outside, p1, t0, t, color, true, file, tabs);
    }
    acc += shape.Xform(t0);

    if (acc)
    {
        ERRBLURB;
        cerr << "problems rendering wire to file\n";
        return -1;
    }

    return 0;
} // extrude()





int Wire::render(Quat &p1, Quat &p2, Quat &p3, Quat &norm, SimplePoly &shape, double &length,
        bool cap0, bool outside, Transform &t, VRMLMat &color, bool reuse,
        std::ofstream &file, int tabs)
{
    /*
     *      + determine the axis about which to rotate
     *      + determine how many segments are required to make the necessary bend
     *      + ensure the minimum bend requirement can be met
     *      + render the straight portion (include transform by t)
     *      + calculate intermediate points, transform and render the curve
     *        (include transform by t)
     *      + take the end point of the curve to use as the first point
     *        in the next triplet or paired point operation
     */

    // determine the axis of rotation
    Quat v0, v1, v2;
    v1 = p2 - p1;
    v2 = p3 - p2;
    v0 = v1.cross(v2);
    v0.vnormalize();

    // ensure there is enough space for the bend
    double l1, l2, l3;
    l1 = sqrt(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
    l2 = sqrt(v2.x*v2.x + v2.y*v2.y + v2.z*v2.z);
    l3 = 0.0;
    if (v0.w > 1.0/seg_ang)
    {
        l3 = rbend/tan((M_PI - v0.w)/2.0);
    }
    // l1 and l2 must be equal to or exceed l3
    if ((l1 < l3) || (l2 < l3))
    {
        ERRBLURB;
        cerr << "path segment is too short and is < length required for bend\n";
        return -1;
    }
    double h;   // hypotenuse
    h = sqrt(l3*l3 + rbend*rbend);

    // number of intermediate points to represent the bend
    int np = (v0.w*seg_ang + 0.1);  // +0.1 helps with rounding errors; better more than less
    if (np < 0)
    {
        ERRBLURB;
        cerr << "BUG: unexpected code execution; transform angle < 0\n";
        return -1;
    }
    ++np;
    if (np == 1) ++np;  // ensure we have a minimum 2 points

    // length from p1 to the last intermediate point; since we
    // want a realistic value, we calculate the length of the
    // wire along an arc rather than approximating by the
    // interpolated line segments. We do not include the length
    // from the final intermediate point to p3 since that length
    // will be calculated in the next call to extrude() or render().
    length += l1 - l3 + rbend*(M_PI - v0.w);

    // 1. calculate the center of rotation
    // 2. calculate the first intermediate point
    // 3. extrude the first segment
    // 4. calculate the shape orientation at the intermediate point

    Quat nv0, nv1, nv2;
    nv1 = v1;
    nv1.vnormalize();
    nv2 = v2;
    nv2.vnormalize();

    Transform t0, t1;
    Quat ip0, ip1, ip2, ip3, ip4, c0;

    ip0 = p1;
    ip3 = p2 - l3*nv1;      // first intermediate point, reference

    ip2 = (nv2 - nv1);
    ip2.vnormalize();
    c0 = p2 + h*ip2;        // center of rotation

    nv0 = norm;
    nv1 = nv0.cross(v1);
    nv1.vnormalize();
    if ((nv1.x*nv1.x + nv1.y*nv1.y + nv1.z*nv1.z) < 1e-9)
    {
        if (nv1.w > 0.0)
        {
            // xxx - BUG: this only applies to vectors along z or x
            // and not to a general vector!
            nv1.x = 0.0;
            nv1.y = 1;
            nv1.z = 0.0;
        }
        else
        {
            nv1 = norm;
        }
    }

    int acc = 0;
    t1.setTranslation(ip3 - p1);
    if (l1 > 0.0)
    {
        acc += shape.Extrude(cap0, false, outside, p1, t1, t, color, reuse, file, tabs);
        cap0 = false;
        // shift the shape to the first intermediate point
        t0.setTranslation(ip3 - p1);
        t0.setRotation(nv1);
        shape.Xform(t0);
    }

    int i;
    ip2 = (ip3 - c0);   // offset from center at c0
    double ang = v0.w/(np -1);
    nv1 = v0;
    nv1.w = ang;
    ip0 = ip3;
    Transform tzero;
    for (i = 1; i < np; ++i)
    {
        // transform the previous point to the new location
        nv2 = v0;
        nv2.w = i*ang;
        t1.setRotation(nv2);
        t1.setTranslation(0,0,0);
        ip1 = ip2;
        t1.transform(ip1);
        ip1 = ip1 - ip2 + ip3;
        // calculate the relative offset
        ip4 = ip1 - ip0;
        // set the transform to place the shape into the new position
        t1.setRotation(nv1);
        t1.setTranslation(ip4);
        acc += shape.Extrude(cap0, false, outside, ip0, t1, t, color, reuse, file, tabs);
        cap0 = false;
        // transform the shape to the new position
        tzero.setRotation(0,0,0,0);
        tzero.setTranslation(-ip0);
        shape.Xform(tzero);
        tzero.setRotation(nv1);
        tzero.setTranslation(ip0 + ip4);
        shape.Xform(tzero);
        ip0 = ip1;
    }
    norm = p3 - p2;
    norm.w = 0.0;
    norm.vnormalize();
    p1 = ip1;
    p2 = p3;
    if (acc)
    {
        ERRBLURB;
        cerr << "problems writing wire shape to file\n";
        return -1;
    }
    return 0;
} // render()


} /* namespace kc3d */
