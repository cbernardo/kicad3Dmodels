/*
 file: transform.cpp

 Copyright 2012-2014 Dr. Cirilo Bernardo (cjh.bernardo@gmail.com)

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

#include <vdefs.h>
#include <transform.h>

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
using namespace KC3D;

/*
 *
 *         BASIC QUATERNION ARITHMETIC
 *
 */
QUAT::QUAT()
{
    x = y = z = 0.0;
    w = 1.0;
    return;
}


QUAT::QUAT( const QUAT &aQuat )
{
    x = aQuat.x;
    y = aQuat.y;
    z = aQuat.z;
    w = aQuat.w;

    return;
}


QUAT::QUAT( double w, double x, double y, double z )
{
    QUAT::w = w;
    QUAT::x = x;
    QUAT::y = y;
    QUAT::z = z;

    return;
}


/* normalize the quaternion */
int QUAT::Normalize( void )
{
    double n = w * w;

    // ensure that w is not too small; a zero value is bad news
    if( n < MIN_W * MIN_W )
    {
        ERRBLURB << "w is smaller than the limit " << MIN_W << "\n";
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
int QUAT::VNormalize( void )
{
    double n = x * x + y * y + z * z;

    if( n < MIN_V )
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


void QUAT::Set( double w, double x, double y, double z )
{
    QUAT::w = w;
    QUAT::x = x;
    QUAT::y = y;
    QUAT::z = z;

    return;
}


QUAT QUAT::operator-()
{
    QUAT local( -w, -x, -y, -z );

    return local;
}


QUAT QUAT::operator+( QUAT arg )
{
    QUAT local( arg );
    local.w += w;
    local.x += x;
    local.y += y;
    local.z += z;

    return local;
}


QUAT QUAT::operator-( QUAT arg )
{
    QUAT local( -arg );
    local.w += w;
    local.x += x;
    local.y += y;
    local.z += z;

    return local;
}


QUAT QUAT::operator*( QUAT arg )
{
    QUAT local( 0, 0, 0, 0 );
    local.w = w * arg.w - x * arg.x - y * arg.y - z * arg.z;
    local.x = w * arg.x + x * arg.w + y * arg.z - z * arg.y;
    local.y = w * arg.y - x * arg.z + y * arg.w + z * arg.x;
    local.z = w * arg.z + x * arg.y - y * arg.x + z * arg.w;

    return local;
}


QUAT QUAT::operator*( double arg )
{
    QUAT local( *this );
    local.w *= arg;
    local.x *= arg;
    local.y *= arg;
    local.z *= arg;

    return local;
}


QUAT QUAT::operator/( double arg )
{
    QUAT local( *this );
    local.w /= arg;
    local.x /= arg;
    local.y /= arg;
    local.z /= arg;

    return local;
}


QUAT KC3D::operator*( double d, QUAT q )
{
    return q * d;
}


QUAT QUAT::Cross( const QUAT &aQuat ) const
{
    QUAT q;
    q.x = y*aQuat.z - z*aQuat.y;
    q.y = z*aQuat.x - x*aQuat.z;
    q.z = x*aQuat.y - y*aQuat.x;

    QUAT p0( *this );
    QUAT p1( aQuat );
    p0.VNormalize();
    p1.VNormalize();

    q.w = acos( p0.x*p1.x + p0.y*p1.y + p0.z*p1.z );
    return q;
}


/*
 *
 *         TRANSLATION
 *
 */
TRANSLATION::~TRANSLATION()
{
    return;
}


TRANSLATION::TRANSLATION()
{
    offset = QUAT( 1, 0, 0, 0 );
    unity = true;

    return;
}


TRANSLATION::TRANSLATION( QUAT aVector )
{
    offset = aVector;
    offset.w = 0.0;
    testUnity();

    return;
}


TRANSLATION::TRANSLATION( double x, double y, double z )
{
    offset = QUAT( 0.0, x, y, z );
    testUnity();

    return;
}


TRANSLATION::TRANSLATION( const TRANSLATION &aVector )
{
    offset = aVector.offset;
    testUnity();

    return;
}


void TRANSLATION::Set( QUAT aVector )
{
    offset.x = aVector.x;
    offset.y = aVector.y;
    offset.z = aVector.z;
    testUnity();

    return;
}


void TRANSLATION::Set( double x, double y, double z )
{
    offset.x = x;
    offset.y = y;
    offset.z = z;
    testUnity();

    return;
}


void TRANSLATION::testUnity( void )
{
    unity = true;

    if( fabs( offset.x ) > MIN_TRANS )
        unity = false;

    if( fabs( offset.y ) > MIN_TRANS )
        unity = false;

    if( fabs( offset.z ) > MIN_TRANS )
        unity = false;

    return;
}


TRANSLATION TRANSLATION::operator-()
{
    TRANSLATION local( -offset.x, -offset.y, -offset.z );

    return local;
}


TRANSLATION TRANSLATION::operator+( QUAT arg )
{
    TRANSLATION local( offset.x + arg.x, offset.y + arg.y, offset.z + arg.z );

    return local;
}


TRANSLATION TRANSLATION::operator+( TRANSLATION arg )
{
    TRANSLATION local( offset.x + arg.offset.x, offset.y + arg.offset.y,
            offset.z + arg.offset.z );

    return local;
}


TRANSLATION TRANSLATION::operator-( QUAT arg )
{
    TRANSLATION local( offset.x - arg.x, offset.y - arg.y, offset.z - arg.z );

    return local;
}


TRANSLATION TRANSLATION::operator-( TRANSLATION arg )
{
    TRANSLATION local( offset.x - arg.offset.x, offset.y - arg.offset.y,
            offset.z - arg.offset.z );

    return local;
}


TRANSLATION TRANSLATION::operator*( double arg )
{
    TRANSLATION local(offset.x*arg, offset.y*arg, offset.z*arg);
    return local;
}


TRANSLATION KC3D::operator*( double d, TRANSLATION t )
{
    TRANSLATION local = t*d;

    return local;
}


TRANSLATION TRANSLATION::operator/( double arg )
{
    TRANSLATION local( offset.x/arg, offset.y/arg,offset.z/arg );

    return local;
}


void TRANSLATION::Translate( QUAT &p )
{
    if( unity )
        return;

    p.x += offset.x;
    p.y += offset.y;
    p.z += offset.z;

    return;
}


void TRANSLATION::Translate( double &x, double &y, double &z )
{
    if( unity )
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
ROTATION::~ROTATION()
{
    return;
}


ROTATION::ROTATION()
{
    zeroRotation();

    return;
}


ROTATION::ROTATION( double angle, double x, double y, double z )
{
    Set( angle, x, y, z );

    return;
}


ROTATION::ROTATION( QUAT aRotation )
{
    Set( aRotation );
    return;
}


bool ROTATION::IsUnity( void )
{
    return unity;
}


void ROTATION::zeroRotation()
{
    axisangle = QUAT( 0.0, 0.0, 0.0, 1.0 );
    mat[1] = mat[2] = mat[3] = mat[5] = mat[6] = mat[7] = 0.0;
    mat[0] = mat[4] = mat[8] = 1.0;
    unity = true;

    return;
}


void ROTATION::Set( QUAT aRotation )
{
    unity = false;
    axisangle = aRotation;
    aRotation.VNormalize();
    double angle, x, y, z, w, n, sina;

    angle = aRotation.w;

    if( (angle > M_PI) || (angle < -M_PI) )
    {
        int i = (int)( angle/M_PI );
        angle = angle - ( M_PI * (double)i );
    }

    axisangle = QUAT( angle, aRotation.x, aRotation.y, aRotation.z );
    axisangle.VNormalize();
    angle /= 2.0;
    // calculate magnitude of axis vector
    n = aRotation.x * aRotation.x + aRotation.y * aRotation.y + aRotation.z * aRotation.z;

    if( ((angle * angle) < MIN_ANGLE) || (n < MIN_V))
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


void ROTATION::Set( double angle, double x, double y, double z )
{
    Set( QUAT( angle, x, y, z ) );

    return;
}


QUAT ROTATION::Get( void )
{
    return axisangle;
}


void ROTATION::Rotate( QUAT &aPoint )
{
    if( unity )
        return;

    Rotate( aPoint.x, aPoint.y, aPoint.z );

    return;
}


void ROTATION::Rotate( double &x, double &y, double &z )
{
    double a, b, c;

    if( unity )
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
SCALE::SCALE()
{
    factor = QUAT( 1.0, 1.0, 1.0, 1.0 );
    unity = true;

    return;
}


SCALE::SCALE( QUAT aScale )
{
    factor = aScale;
    factor.w = 1.0;
    testUnity();

    return;
}


SCALE::SCALE( double xScale, double yScale, double zScale )
{
    factor = QUAT( 1.0, xScale, yScale, zScale );
    testUnity();

    return;
}


void SCALE::Set( double xScale, double yScale, double zScale )
{
    factor = QUAT( 1.0, xScale, yScale, zScale );
    testUnity();

    return;
}


void SCALE::testUnity()
{
    unity = true;

    if( fabs( factor.x - 1.0 ) > MIN_SCALE )
        unity = false;

    if( fabs( factor.y - 1.0 ) > MIN_SCALE )
        unity = false;

    if( fabs( factor.z - 1.0 ) > MIN_SCALE )
        unity = false;

    return;
}


void SCALE::Scale( QUAT &aPoint )
{
    if( unity )
        return;

    aPoint.w *= factor.w;
    aPoint.x *= factor.x;
    aPoint.y *= factor.y;
    aPoint.z *= factor.z;

    return;
}


void SCALE::Scale( double &x, double &y, double &z )
{
    if( unity )
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
TRANSFORM::TRANSFORM()
{
    unity = true;

    return;
}


TRANSFORM::TRANSFORM( TRANSLATION T, ROTATION R, SCALE S )
{
    t = T;
    r = R;
    s = S;
    testUnity();

    return;
}


void TRANSFORM::Set( TRANSLATION T, ROTATION R, SCALE S )
{
    t = T;
    r = R;
    s = S;
    testUnity();

    return;
}


void TRANSFORM::testUnity( void )
{
    if( t.IsUnity() && r.IsUnity() && s.IsUnity() )
        unity = true;
    else
        unity = false;

    return;
}


void TRANSFORM::SetTranslation( TRANSLATION T )
{
    t = T;
    testUnity();

    return;
}


void TRANSFORM::SetTranslation( QUAT aTranslation )
{
    t = TRANSLATION( aTranslation );
    testUnity();

    return;
}


void TRANSFORM::SetTranslation( double x, double y, double z )
{
    t = TRANSLATION( x, y, z );
    testUnity();

    return;
}


void TRANSFORM::SetRotation( ROTATION R )
{
    r = R;
    testUnity();

    return;
}


void TRANSFORM::SetRotation( QUAT aRotation )
{
    r = ROTATION( aRotation );
    testUnity();

    return;
}


void TRANSFORM::SetRotation( double angle, double x, double y, double z )
{
    r = ROTATION( angle, x, y, z );
    testUnity();

    return;
}


void TRANSFORM::SetScale( SCALE S )
{
    s = S;
    testUnity();

    return;
}


void TRANSFORM::SetScale( QUAT aScale )
{
    s = SCALE( aScale );
    testUnity();

    return;
}


void TRANSFORM::SetScale( double x, double y, double z )
{
    s = SCALE( x, y, z );
    testUnity();

    return;
}


void TRANSFORM::SetScale( double aScale )
{
    s = SCALE( aScale, aScale, aScale );
    testUnity();

    return;
}


void TRANSFORM::Transform( QUAT &aPoint )
{
    if( unity )
        return;

    s.Scale( aPoint );
    r.Rotate( aPoint );
    t.Translate( aPoint );

    return;
}


void TRANSFORM::Transform( double &x, double &y, double &z )
{
    if( unity )
        return;

    s.Scale( x, y, z );
    r.Rotate( x, y, z );
    t.Translate( x, y, z );

    return;
}


void TRANSFORM::Transform( QUAT *aPointList, int nPoints )
{
    int i;

    if( unity )
        return;

    for ( i = 0; i < nPoints; ++i )
    {
        s.Scale( *aPointList );
        r.Rotate( *aPointList );
        t.Translate( *aPointList++ );
    }

    return;
}


void TRANSFORM::Transform( double *x, double *y, double *z, int nPoints )
{
    int i;
    double *X, *Y, *Z;

    X = x;
    Y = y;
    Z = z;

    if( unity )
        return;

    for (i = 0; i < nPoints; ++i)
    {
        s.Scale( *X, *Y, *Z );
        r.Rotate( *X, *Y, *Z );
        t.Translate( *X, *Y, *Z );
        ++X;
        ++Y;
        ++Z;
    }

    return;
}


/// TODO: Document and push into python bindings
void TRANSFORM::GetTranslation( TRANSLATION &aTranslation )
{
    aTranslation = t;
    return;
}


/// TODO: Document and push into python bindings
void TRANSFORM::GetRotation( ROTATION &aRotation )
{
    aRotation = r;
    return;
}


/// TODO: Document and push into python bindings
void TRANSFORM::GetScale( SCALE &aScale )
{
    aScale = s;
    return;
}
