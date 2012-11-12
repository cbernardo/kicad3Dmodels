/*
 file: transform.cpp

 Copyright 2012, Cirilo Bernardo

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>

 Notes:
 These classes were created to support the
 programmatic creation of VRML models for
 use with KiCad.

 */

#include <iostream>
#include <cstdlib>
#include <cmath>

#include "transform.h"

// the minimum acceptable value of w in a quaternion normalization
#define MIN_W (1e-9)
// the minimum acceptable vector magnitude in a normalization
#define MIN_V (1e-24)
// the minimum angle squared in a rotation
#define MIN_ANGLE (1e-12)
// minimum X, Y, Z scale factor accepted as non-unity
#define MIN_SCALE (1e-8)
// minimum X, Y, Z value accepted as non-unity translation
#define MIN_TRANS (1e-8)

using namespace std;
using namespace kc3d;

/*
 *
 *         BASIC QUATERNION ARITHMETIC
 *
 */
Quat::Quat()
{
    x = y = z = 0.0;
    w = 1.0;
    return;
}

Quat::Quat(const Quat &p)
{
    x = p.x;
    y = p.y;
    z = p.z;
    w = p.w;
    return;
}

Quat::Quat(double w, double x, double y, double z)
{
    Quat::w = w;
    Quat::x = x;
    Quat::y = y;
    Quat::z = z;
    return;
}


/* normalize the quaternion */
int Quat::normalize(void)
{
    double n = w * w;

    // ensure that w is not too small; a zero value is bad news
    if (n < MIN_W * MIN_W)
    {
        cerr << __FUNCTION__ << ":" << __LINE__ << ": " << __FUNCTION__
                << "(): w is smaller than the limit " << MIN_W << "\n";
        return -1;
    }

    // compute the normalization factor
    n += x * x + y * y + z * z;
    n = sqrt(n);
    w /= n;
    x /= n;
    y /= n;
    z /= n;
    return 0;
}

/* normalize the XYZ vector */
int Quat::vnormalize(void)
{
    double n = x * x + y * y + z * z;
    if (n < MIN_V)
    {
        x = y = z = 0.0;
        return 1;
    }
    n = sqrt(n);
    x /= n;
    y /= n;
    z /= n;
    return 0;
}

Quat Quat::operator-()
{
    Quat local(-w, -x, -y, -z);
    return local;
}

Quat Quat::operator+(Quat arg)
{
    Quat local(arg);
    local.w += w;
    local.x += x;
    local.y += y;
    local.z += z;
    return local;
}

Quat Quat::operator-(Quat arg)
{
    Quat local(-arg);
    local.w += w;
    local.x += x;
    local.y += y;
    local.z += z;
    return local;
}

Quat Quat::operator*(Quat arg)
{
    Quat local(0, 0, 0, 0);
    local.w = w * arg.w - x * arg.x - y * arg.y - z * arg.z;
    local.x = w * arg.x + x * arg.w + y * arg.z - z * arg.y;
    local.y = w * arg.y - x * arg.z + y * arg.w + z * arg.x;
    local.z = w * arg.z + x * arg.y - y * arg.x + z * arg.w;
    return local;
}

Quat Quat::operator*(double arg)
{
    Quat local(*this);
    local.w *= arg;
    local.x *= arg;
    local.y *= arg;
    local.z *= arg;
    return local;
}

Quat Quat::operator/(double arg)
{
    Quat local(*this);
    local.w /= arg;
    local.x /= arg;
    local.y /= arg;
    local.z /= arg;
    return local;
}

Quat kc3d::operator*(double d, Quat q)
{
    return q * d;
}

/*
 *
 *         TRANSLATION
 *
 */
Translation::~Translation()
{
    return;
}

Translation::Translation()
{
    offset = Quat(1, 0, 0, 0);
    unity = true;
    return;
}

Translation::Translation(Quat p)
{
    offset = p;
    offset.w = 0.0;
    testUnity();
    return;
}

Translation::Translation(double x, double y, double z)
{
    offset = Quat(0.0, x, y, z);
    testUnity();
    return;
}

Translation::Translation(const Translation &t)
{
    offset = t.offset;
    testUnity();
    return;
}

void Translation::set(Quat t)
{
    offset.x = t.x;
    offset.y = t.y;
    offset.z = t.z;
    testUnity();
    return;
}

void Translation::set(double x, double y, double z)
{
    offset.x = x;
    offset.y = y;
    offset.z = z;
    testUnity();
    return;
}

void Translation::testUnity(void)
{
    unity = true;
    if (fabs(offset.x) > MIN_TRANS)
        unity = false;
    if (fabs(offset.y) > MIN_TRANS)
        unity = false;
    if (fabs(offset.z) > MIN_TRANS)
        unity = false;
    return;
}

Translation Translation::operator-()
{
    Translation local(-offset.x, -offset.y, -offset.z);
    return local;
}

Translation Translation::operator+(Quat arg)
{
    Translation local(offset.x + arg.x, offset.y + arg.y, offset.z + arg.z);
    return local;
}

Translation Translation::operator+(Translation arg)
{
    Translation local(offset.x + arg.offset.x, offset.y + arg.offset.y,
            offset.z + arg.offset.z);
    return local;
}

Translation Translation::operator-(Quat arg)
{
    Translation local(offset.x - arg.x, offset.y - arg.y, offset.z - arg.z);
    return local;
}

Translation Translation::operator-(Translation arg)
{
    Translation local(offset.x - arg.offset.x, offset.y - arg.offset.y,
            offset.z - arg.offset.z);
    return local;
}

Translation Translation::operator*(double arg)
{
    Translation local(offset.x*arg, offset.y*arg, offset.z*arg);
    return local;
}

Translation kc3d::operator*(double d, Translation t)
{
    Translation local = t*d;
    return local;
}

Translation Translation::operator/(double arg)
{
    Translation local(offset.x/arg, offset.y/arg,offset.z/arg);
    return local;
}

void Translation::translate(Quat &p)
{
    if (unity)
        return;
    p.x += offset.x;
    p.y += offset.y;
    p.z += offset.z;
    return;
}

void Translation::translate(double &x, double &y, double &z)
{
    if (unity)
        return;
    x += offset.x;
    y += offset.y;
    z += offset.z;
    return;
}



/*
 *
 *         ROTATION
 *
 */
Rotation::~Rotation()
{
    return;
}

Rotation::Rotation()
{
    zeroRotation();
    return;
}

Rotation::Rotation(double angle, double x, double y, double z)
{
    set(angle, x, y, z);
    return;
}

Rotation::Rotation(Quat p)
{
    set(p);
    return;
}

bool Rotation::isUnity(void)
{
    return unity;
}

void Rotation::zeroRotation()
{
    axisangle = Quat(0.0, 0.0, 0.0, 1.0);
    mat[1] = mat[2] = mat[3] = mat[5] = mat[6] = mat[7] = 0.0;
    mat[0] = mat[4] = mat[8] = 1.0;
    unity = true;
    return;
}

void Rotation::set(Quat pt)
{
    unity = false;
    axisangle = pt;
    pt.vnormalize();
    double angle, x, y, z, w, n, sina;

    angle = pt.w;
    if ((angle > M_PI)||(angle < -M_PI))
    {
        int i = (int)(angle/M_PI);
        angle = angle - (M_PI*(double)i);
    }
    axisangle = Quat(angle, pt.x, pt.y, pt.z);
    axisangle.vnormalize();
    angle /= 2.0;
    n = pt.x * pt.x + pt.y * pt.y + pt.z * pt.z;  // magnitude of axis vector
    if (((angle * angle) < MIN_ANGLE)||(n < MIN_V))
    {
        zeroRotation();
        return;
    }

    w = cos(angle);
    sina = sin(angle);
    x = axisangle.x * sina;
    y = axisangle.y * sina;
    z = axisangle.z * sina;

    mat[0] = 1.0 - 2.0 * (y * y + z * z);
    mat[1] = 2.0 * (x * y - w * z);
    mat[2] = 2.0 * (x * z + w * y);
    mat[3] = 2.0 * (x * y + w * z);
    mat[4] = 1.0 - 2.0 * (x * x + z * z);
    mat[5] = 2.0 * (y * z - w * x);
    mat[6] = 2.0 * (x * z - w * y);
    mat[7] = 2.0 * (y * z + w * x);
    mat[8] = 1.0 - 2.0 * (x * x + y * y);
    return;
}

void Rotation::set(double angle, double x, double y, double z)
{
    set(Quat(angle, x, y, z));
    return;
}

Quat Rotation::get(void)
{
    return axisangle;
}

void Rotation::rotate(Quat &pt)
{
    if (unity)
        return;
    rotate(pt.x, pt.y, pt.z);
    return;
}

void Rotation::rotate(double &x, double &y, double &z)
{
    double a, b, c;
    if (unity)
        return;
    a = mat[0] * x + mat[1] * y + mat[2] * z;
    b = mat[3] * x + mat[4] * y + mat[5] * z;
    c = mat[6] * x + mat[7] * y + mat[8] * z;
    x = a;
    y = b;
    z = c;
    return;
}

/*
 *
 *         SCALE
 *
 */
Scale::Scale()
{
    factor = Quat(1.0, 1.0, 1.0, 1.0);
    unity = true;
    return;
}

Scale::Scale(Quat p)
{
    factor = p;
    factor.w = 1.0;
    testUnity();
    return;
}

Scale::Scale(double xscale, double yscale, double zscale)
{
    factor = Quat(1.0, xscale, yscale, zscale);
    testUnity();
    return;
}

void Scale::set(double xscale, double yscale, double zscale)
{
    factor = Quat(1.0, xscale, yscale, zscale);
    testUnity();
    return;
}

void Scale::testUnity()
{
    unity = true;
    if (fabs(factor.x - 1.0) > MIN_SCALE)
        unity = false;
    if (fabs(factor.y - 1.0) > MIN_SCALE)
        unity = false;
    if (fabs(factor.z - 1.0) > MIN_SCALE)
        unity = false;
    return;
}

void Scale::scale(Quat &p)
{
    if (unity)
        return;
    p.w *= factor.w;
    p.x *= factor.x;
    p.y *= factor.y;
    p.z *= factor.z;
    return;
}

void Scale::scale(double &x, double &y, double &z)
{
    if (unity)
        return;
    x *= factor.x;
    y *= factor.y;
    z *= factor.z;
    return;
}

/*
 *
 *         TRANSFORM
 *
 */
Transform::Transform()
{
    unity = true;
}

Transform::Transform(Translation T, Rotation R, Scale S)
{
    t = T;
    r = R;
    s = S;
    testUnity();
}

void Transform::set(Translation T, Rotation R, Scale S)
{
    t = T;
    r = R;
    s = S;
    testUnity();
}

void Transform::testUnity(void)
{
    if (t.isUnity() && r.isUnity() && s.isUnity())
        unity = true;
    else
        unity = false;
}

void Transform::setTranslation(Translation T)
{
    t = T;
    testUnity();
}

void Transform::setTranslation(Quat q)
{
    t = Translation(q);
    testUnity();
}

void Transform::setTranslation(double x, double y, double z)
{
    t = Translation(x, y, z);
    testUnity();
}

void Transform::setRotation(Rotation R)
{
    r = R;
    testUnity();
}

void Transform::setRotation(Quat q)
{
    r = Rotation(q);
    testUnity();
}

void Transform::setRotation(double angle, double x, double y, double z)
{
    r = Rotation(angle, x, y, z);
    testUnity();
}

void Transform::setScale(Scale S)
{
    s = S;
    testUnity();
}

void Transform::setScale(Quat q)
{
    s = Scale(q);
    testUnity();
}

void Transform::setScale(double x, double y, double z)
{
    s = Scale(x, y, z);
    testUnity();
}

void Transform::setScale(double n)
{
    s = Scale(n, n, n);
    testUnity();
}

void Transform::transform(Quat &p)
{
    if (unity)
        return;
    r.rotate(p);
    s.scale(p);
    t.translate(p);
    return;
}

void Transform::transform(double &x, double &y, double &z)
{
    if (unity)
        return;
    r.rotate(x, y, z);
    s.scale(x, y, z);
    t.translate(x, y, z);
    return;
}

void Transform::transform(Quat *p, int np)
{
    int i;
    Quat *qp = p;

    if (unity)
        return;
    for (i = 0; i < np; ++i)
    {
        r.rotate(*qp);
        s.scale(*qp);
        t.translate(*qp++);
    }
}

void Transform::transform(double *x, double *y, double *z, int np)
{
    int i;
    double *X, *Y, *Z;

    X = x;
    Y = y;
    Z = z;

    if (unity)
        return;
    for (i = 0; i < np; ++i)
    {
        r.rotate(*X, *Y, *Z);
        t.translate(*X, *Y, *Z);
        s.scale(*X, *Y, *Z);
        ++X;
        ++Y;
        ++Z;
    }
}


// TODO: Document and push into python bindings
void Transform::GetTranslation(Translation &extt)
{
    extt = t;
    return;
}

// TODO: Document and push into python bindings
void Transform::GetRotation(Rotation &extr)
{
    extr = r;
    return;
}

// TODO: Document and push into python bindings
void Transform::GetScale(Scale &exts)
{
    exts = s;
    return;
}

