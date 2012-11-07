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

namespace kc3d {

/** \file */

/// Value of PI used for internal calculations; this is sufficient for 80-bit floats
#ifndef M_PI
    #define M_PI 3.1415926535897932384626433832795
#endif

/**
 * \ingroup common_tools
 * \brief Basic Quaternion
 *
 * This is a basic Quaternion implementation providing
 * operators which are used for geometric transformations.
 */
class Quat
{
public:
    double w, x, y, z;  ///< In mathematical terms (u, v) is
                        ///< represented by (w, (x, y, z))

    Quat();
    Quat(const Quat &p);
    Quat(double w, double x, double y, double z);

    /// Negative of a quaternion
    Quat operator-();
    /// Sum of two quaternions
    Quat operator+(Quat arg);
    /// Difference between two quaternions
    Quat operator-(Quat arg);
    /// Product of two Quaternions; the product is generally non-commutative.
    Quat operator*(Quat arg);
    /// Product of a Quaternion and a scalar
    Quat operator*(double arg);
    /// Quaternion divided by a scalar
    Quat operator/(double arg);

    /**
     * Normalize the Quaternion.
     *
     * @return 0 for success, -1 for failure
     */
    int normalize(void);

    /**
     * Normalize the XYZ vector.
     *
     * @return 0 for success, -1 for failure
     */
    int vnormalize(void);
};
/// Product of a scalar and a Quaternion
Quat operator*(double d, Quat q);



/**
 *  * \ingroup common_tools
 * \brief Geometric Translation
 *
 *  This class stores information for and
 *  performs geometric translations on 3D point
 *  representations.
 */
class Translation
{
protected:
    Quat offset;            ///< 3D offset to apply to a point
    bool unity;             ///< TRUE when the translation represents an identity transform
    void testUnity(void);   ///< Performs a test and sets the @var unity flag

public:
    virtual ~Translation();
    Translation();
    Translation(Quat p);
    Translation(const Translation &p);
    Translation(double x, double y, double z);

    /// Negative of the translation
    Translation operator-();
    /// Sum of two translations
    Translation operator+(Quat arg);
    /// Sum of two translations
    Translation operator+(Translation arg);
    /// Difference of two translations
    Translation operator-(Quat arg);
    /// Difference of two translations
    Translation operator-(Translation arg);
    /// Product of a Translation and a scalar
    Translation operator*(double arg);
    /// Translation divided by a scalar
    Translation operator/(double arg);

    /**
     * Set the translation parameters via a Quaternion
     *
     * @param t [in] Translation parameters; the @var w component of the Quaternion is ignored
     */
    void set(Quat t);
    /**
     * Set the translation parameters via explicit X Y Z values
     *
     * @param x [in] X offset
     * @param y [in] Y offset
     * @param z [in] Z offset
     */
    void set(double x, double y, double z);
    /**
     * Perform a translation on a point represented by a Quaternion.
     *
     * @param pt [in,out] point to be translated
     */
    void translate(Quat &pt);
    /**
     * Perform a translation on a point represented by three doubles.
     *
     * @param x [in,out]
     * @param y [in,out]
     * @param z [in,out]
     */
    void translate(double &x, double &y, double &z);
    /**
     * Test if the transform is a unity transform
     *
     * @return TRUE if the translation is unity (0, 0, 0)
     */
    bool isUnity(void)
    {
        return unity;
    }
};
/**
 * Product of a scalar and a translation
 *
 * @param d [in]
 * @param t [in]
 * @return modified (scaled) Translation
 */
Translation operator*(double d, Translation t);



/**
 *  * \ingroup common_tools
 * \brief Geometric Rotation
 *
 *  This class stores information for and
 *  performs geometric rotations on 3D point
 *  representations.  The reference origin
 *  is (0, 0, 0).
 */
class Rotation
{
private:
    Quat axisangle;         ///< Rotation angle and orientation vector
    bool unity;             ///< TRUE if the transform is a unity transform
    double mat[9];          ///< Internal matrix for rotation calculations
    void zeroRotation();    ///< Set the rotation matrix to zero rotation

public:
    virtual ~Rotation();
    Rotation();
    Rotation(double angle, double x, double y, double z);
    Rotation(Quat p);

    /**
     * Perform a rotation on a point represented by a Quaternion
     *
     * @param pt [in,out]
     */
    void rotate(Quat &pt);
    /**
     * Perform a rotation on a point represented by discrete doubles
     *
     * @param x [in,out]
     * @param y [in,out]
     * @param z [in,out]
     */
    void rotate(double &x, double &y, double &z);
    /**
     * Set the angle and orientation vectors.
     * The rotation parameters are internally normalized.
     *
     * @param pt [in]
     */
    void set(Quat pt);
    /**
     * Set the angle and orientation vectors
     * The rotation parameters are internally normalized.
     *
     * @param angle
     * @param x
     * @param y
     * @param z
     */
    void set(double angle, double x, double y, double z);
    /**
     * Retrieve the normalized Quaternion representing the rotation
     * @return Normalized rotation parameters
     */
    Quat get(void);
    /**
     * Test if the transform is a unity transform
     *
     * @return TRUE if the rotation is unity
     */
    bool isUnity(void);
};



/**
 *  * \ingroup common_tools
 * \brief Geometric Scale
 *
 *  This class stores information for and
 *  performs geometric scaling on 3D point
 *  representations.  The reference origin
 *  is (0, 0, 0).
 */
class Scale
{
private:
    Quat factor;        ///< X, Y, and Z scale factors
    bool unity;         ///< TRUE if the transform is a unity transform
    void testUnity();   ///< sets unity to TRUE if scale is approx 1,1,1

public:
    Scale();
    Scale(Quat p);
    Scale(double xscale, double yscale, double zscale);

    /**
     * Set the scales on each axis.
     *
     * @param xscale [in]
     * @param yscale [in]
     * @param zscale [in]
     */
    void set(double xscale, double yscale, double zscale);

    /**
     * Scale a point represented by a Quaternion
     *
     * @param p [in,out]
     */
    void scale(Quat &p);

    /**
     * Scale a point represented by discrete coordinates
     *
     * @param x [in,out]
     * @param y [in,out]
     * @param z [in,out]
     */
    void scale(double &x, double &y, double &z);
    /**
     * Test if the transform is a unity transform
     *
     * @return TRUE if the scale factors are unity
     */
    bool isUnity(void)
    {
        return unity;
    }
};



/**
 *  * \ingroup common_tools
 * \brief Geometric Transform
 *
 *  This class stores information for and performs a geometric
 *  transform on a set of 3D points. The reference origin
 *  is (0, 0, 0). The transformations are performed in the
 *  order (1)Rotation, (2)Translation, (3)Scale
 */
class Transform
{
    Translation t;          ///< translation parameters
    Rotation r;             ///< rotation parameters
    Scale s;                ///< scaling parameters
    bool unity;             ///< TRUE if the transform is a unity transform
    void testUnity(void);   ///< Sets the @var unity flag if the transform is unity

public:
    Transform();
    Transform(Translation T, Rotation R, Scale S);

    /**
     * Set all transform parameters
     *
     * @param T [in]
     * @param R [in]
     * @param S [in]
     */
    void set(Translation T, Rotation R, Scale S);
    /**
     * Set the Translation parameter
     *
     * @param T [in]
     */
    void setTranslation(Translation T);
    /**
     * Set the Translation parameter
     *
     * @param q [in]
     */
    void setTranslation(Quat q);
    /**
     * Set the Translation parameter
     *
     * @param x [in]
     * @param y [in]
     * @param z [in]
     */
    void setTranslation(double x, double y, double z);

    /**
     * Set the rotation parameters
     *
     * @param R [in]
     */
    void setRotation(Rotation R);
    /**
     * Set the rotation parameters
     *
     * @param q [in]
     */
    void setRotation(Quat q);
    /**
     * Set the rotation parameters
     *
     * @param angle [in]
     * @param x     [in]
     * @param y     [in]
     * @param z     [in]
     */
    void setRotation(double angle, double x, double y, double z);

    /**
     * Set the scaling parameters
     *
     * @param S [in]
     */
    void setScale(Scale S);
    /**
     * Set the scaling parameters
     *
     * @param q [in]
     */
    void setScale(Quat q);
    /**
     * Set the scaling parameters
     *
     * @param x [in]
     * @param y [in]
     * @param z [in]
     */
    void setScale(double x, double y, double z);
    /**
     * Set the scaling parameters
     *
     * @param n [in] Uniform scaling to apply to X Y Z axes
     */
    void setScale(double n);

    /**
     * Transform the point represented by a quaternion
     *
     * @param p [in,out]
     */
    void transform(Quat &p);
    /**
     * Transform the point represented by discrete coordinates
     *
     * @param x [in,out]
     * @param y [in,out]
     * @param z [in,out]
     */
    void transform(double &x, double &y, double &z);
    /**
     * Transform a set of points represented by quaternions
     *
     * @param p  [in,out] array of points
     * @param np [in] number of points in the array @var p
     */
    void transform(Quat *p, int np);
    /**
     * Transform a set of points represented by discrete coordinates
     *
     * @param x   [in,out]
     * @param y   [in,out]
     * @param z   [in,out]
     * @param np  [in] number of points to transform
     */
    void transform(double *x, double *y, double *z, int np);
};


}   // namespace kc3d

#endif // TRANSFORM_H
