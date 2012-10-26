/*
 file: transform.h

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

    The coordinate system is a right-hand coordinate system
    in X, Y, Z so it can be imagined, for example, as the
    +X extending from left to right on the screen while +Y
    extends behind the screen and +z extends upwards on the screen.

 */

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <cmath>

#ifndef M_PI
    #define M_PI 3.1415926535897932384626433832795
#endif

class Quat
{
public:
    double w, x, y, z;

    Quat();
    Quat(const Quat &p);
    Quat(double w, double x, double y, double z);

    Quat operator-();
    Quat operator+(Quat arg);
    Quat operator-(Quat arg);
    Quat operator*(Quat arg);
    Quat operator*(double arg);
    Quat operator/(double arg);

    int normalize(void);  ///< normalizes the quaternion
    int vnormalize(void); ///< normalizes the XYZ vector
};
Quat operator*(double d, Quat q);

class Translation
{
protected:
    Quat offset;
    bool unity;
    void testUnity(void);

public:
    virtual ~Translation();
    Translation();
    Translation(Quat p);
    Translation(const Translation &p);
    Translation(double x, double y, double z);

    Translation operator-();
    Translation operator+(Quat arg);
    Translation operator+(Translation arg);
    Translation operator-(Quat arg);
    Translation operator-(Translation arg);
    Translation operator*(double arg);
    Translation operator/(double arg);

    void set(Quat t);
    void set(double x, double y, double z);
    void translate(Quat &pt);
    void translate(double &x, double &y, double &z);
    bool isUnity(void)
    {
        return unity;
    }
};
Translation operator*(double d, Translation t);

class Rotation
{
private:
    Quat axisangle;
    bool unity;
    double mat[9];
    void zeroRotation();  ///< Set the rotation matrix to zero rotation

public:
    virtual ~Rotation();
    Rotation();
    Rotation(double angle, double x, double y, double z);
    Rotation(Quat p);

    void rotate(Quat &pt);
    void rotate(double &x, double &y, double &z);
    void set(Quat pt);
    void set(double angle, double x, double y, double z);
    Quat get(void);
    bool isUnity(void);
};

class Scale
{
private:
    Quat factor;
    bool unity;
    void testUnity(); ///< sets unity to TRUE if scale is approx 1,1,1

public:
    Scale();
    Scale(Quat p);
    Scale(double xscale, double yscale, double zscale);
    void set(double xscale, double yscale, double zscale);

    void scale(Quat &p);
    void scale(double &x, double &y, double &z);
    bool isUnity(void)
    {
        return unity;
    }
};

class Transform
{
    Translation t;
    Rotation r;
    Scale s;
    bool unity;
    void testUnity(void);

public:
    Transform();
    Transform(Translation T, Rotation R, Scale S);
    void set(Translation T, Rotation R, Scale S);
    void setTranslation(Translation T);
    void setTranslation(Quat q);
    void setTranslation(double x, double y, double z);
    void setRotation(Rotation R);
    void setRotation(Quat q);
    void setRotation(double angle, double x, double y, double z);
    void setScale(Scale S);
    void setScale(Quat q);
    void setScale(double x, double y, double z);
    void setScale(double n);

    void transform(Quat &p);
    void transform(double &x, double &y, double &z);
    void transform(Quat *p, int np);
    void transform(double *x, double *y, double *z, int np);
};

#endif // TRANSFORM_H
