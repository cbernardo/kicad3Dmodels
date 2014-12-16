/*
 *  file: transform.h
 *
 *  Copyright 2012-2014 Dr. Cirilo Bernardo (cjh.bernardo@gmail.com)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 *  Notes:
 *   These classes were created to support the
 *   programmatic creation of VRML models for
 *   use with KiCad.
 *
 *   The coordinate system is a right-hand coordinate system
 *   in X, Y, Z so it can be imagined, for example, as the
 *   +X extending from left to right on the screen while +Y
 *   extends behind the screen and +z extends upwards on the screen.
 *
 *   For consistency with other software the geometric transforms are
 *   performed in the order Scale, Rotate, Translate.
 *
 */

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <cmath>

namespace KC3D
{
/** \file */

/// Value of PI used for internal calculations; this is sufficient for 128-bit floats
#ifndef M_PI
    #define M_PI 3.1415926535897932384626433832795028841
#endif

/**
 * \ingroup common_tools
 * \brief Basic Quaternion
 *
 * This is a basic Quaternion implementation providing
 * operators which are used for geometric transformations.
 */
class QUAT
{
public:
    double w, x, y, z;      ///< In mathematical terms (u, v) is
                            ///< represented by (w, (x, y, z))

    QUAT();
    QUAT( const QUAT& aQuat );
    QUAT( double w, double x, double y, double z );

    /// Negative of a quaternion
    QUAT operator-();

    /// Sum of two quaternions
    QUAT operator+( QUAT arg );

    /// Difference between two quaternions
    QUAT operator-( QUAT arg );

    /// Product of two Quaternions; the product is generally non-commutative.
    QUAT operator*( QUAT arg );

    /// Product of a Quaternion and a scalar
    QUAT operator*( double arg );

    /// Quaternion divided by a scalar
    QUAT operator/( double arg );

    /**
     * Normalize the Quaternion.
     *
     * @return 0 for success, -1 for failure
     */
    int Normalize( void );

    /**
     * Normalize the XYZ vector.
     *
     * @return 0 for success, -1 for failure
     */
    int VNormalize( void );

    /**
     * Set the value of the quaternion with discrete numbers
     *
     * @param w
     * @param x
     * @param y
     * @param z
     */
    void Set( double w, double x, double y, double z );

    /**
     * Calculate the cross-product and angle of
     * two vectors (*this x @param aQuat).
     * The result is not normalized.
     *
     * @param aQuat
     * @return cross product and angle
     */
    QUAT Cross( const QUAT& aQuat ) const;
};


/// Product of a scalar and a Quaternion
QUAT operator*( double d, QUAT q );


/**
 *  * \ingroup common_tools
 * \brief Geometric Translation
 *
 *  This class stores information for and
 *  performs geometric translations on 3D point
 *  representations.
 */
class TRANSLATION
{
protected:
    QUAT    offset;         ///< 3D offset to apply to a point
    bool    unity;          ///< TRUE when the translation represents an identity transform
    void testUnity( void ); ///< Performs a test and sets the @var unity flag

public:
    virtual ~TRANSLATION();
    TRANSLATION();
    TRANSLATION( QUAT aVector );
    TRANSLATION( const TRANSLATION& aVector );
    TRANSLATION( double x, double y, double z );

    /// Negative of the translation
    TRANSLATION operator-();

    /// Sum of two translations
    TRANSLATION operator+( QUAT arg );

    /// Sum of two translations
    TRANSLATION operator+( TRANSLATION arg );

    /// Difference of two translations
    TRANSLATION operator-( QUAT arg );

    /// Difference of two translations
    TRANSLATION operator-( TRANSLATION arg );

    /// Product of a Translation and a scalar
    TRANSLATION operator*( double arg );

    /// Translation divided by a scalar
    TRANSLATION operator/( double arg );

    /**
     * Set the translation parameters via a Quaternion
     *
     * @param aVector [in] Translation parameters; the @var w component of the Quaternion is ignored
     */
    void Set( QUAT aVector );

    /**
     * Set the translation parameters via explicit X Y Z values
     *
     * @param x [in] X offset
     * @param y [in] Y offset
     * @param z [in] Z offset
     */
    void Set( double x, double y, double z );

    /**
     * Perform a translation on a point represented by a Quaternion.
     *
     * @param pt [in,out] point to be translated
     */
    void Translate( QUAT& pt );

    /**
     * Perform a translation on a point represented by three doubles.
     *
     * @param x [in,out]
     * @param y [in,out]
     * @param z [in,out]
     */
    void Translate( double& x, double& y, double& z );

    /**
     * Test if the transform is a unity transform
     *
     * @return TRUE if the translation is unity (0, 0, 0)
     */
    bool IsUnity( void )
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
TRANSLATION operator*( double d, TRANSLATION t );


/**
 * \ingroup common_tools
 * \brief Geometric Rotation
 *
 *  This class stores information for and
 *  performs geometric rotations on 3D point
 *  representations.  The reference origin
 *  is (0, 0, 0).
 */
class ROTATION
{
private:
    QUAT    axisangle;      ///< Rotation angle and orientation vector
    bool    unity;          ///< TRUE if the transform is a unity transform
    double mat[9];          ///< Internal matrix for rotation calculations
    void zeroRotation();    ///< Set the rotation matrix to zero rotation

public:
    virtual ~ROTATION();
    ROTATION();
    ROTATION( double angle, double x, double y, double z );
    ROTATION( QUAT aRotation );

    /**
     * Perform a rotation on a point represented by a Quaternion
     *
     * @param aPoint [in,out]
     */
    void Rotate( QUAT& aPoint );

    /**
     * Perform a rotation on a point represented by discrete doubles
     *
     * @param x [in,out]
     * @param y [in,out]
     * @param z [in,out]
     */
    void Rotate( double& x, double& y, double& z );

    /**
     * Set the angle and orientation vectors.
     * The rotation parameters are internally normalized.
     *
     * @param aRotation [in]
     */
    void Set( QUAT aRotation );

    /**
     * Set the angle and orientation vectors
     * The rotation parameters are internally normalized.
     *
     * @param angle
     * @param x
     * @param y
     * @param z
     */
    void Set( double angle, double x, double y, double z );

    /**
     * Retrieve the normalized Quaternion representing the rotation
     * @return Normalized rotation parameters
     */
    QUAT Get( void );

    /**
     * Test if the transform is a unity transform
     *
     * @return TRUE if the rotation is unity
     */
    bool IsUnity( void );
};


/**
 * \ingroup common_tools
 * \brief Geometric Scale
 *
 *  This class stores information for and
 *  performs geometric scaling on 3D point
 *  representations.  The reference origin
 *  is (0, 0, 0).
 */
class SCALE
{
private:
    QUAT    factor;     ///< X, Y, and Z scale factors
    bool    unity;      ///< TRUE if the transform is a unity transform
    void testUnity();   ///< sets unity to TRUE if scale is approx 1,1,1

public:
    SCALE();
    SCALE( QUAT aScale );
    SCALE( double xScale, double yScale, double zScale );

    /**
     * Set the scales on each axis.
     *
     * @param xScale [in]
     * @param yScale [in]
     * @param zScale [in]
     */
    void Set( double xScale, double yScale, double zScale );

    /**
     * Scale a point represented by a Quaternion
     *
     * @param aPoint [in,out]
     */
    void Scale( QUAT& aPoint );

    /**
     * Scale a point represented by discrete coordinates
     *
     * @param x [in,out]
     * @param y [in,out]
     * @param z [in,out]
     */
    void Scale( double& x, double& y, double& z );

    /**
     * Test if the transform is a unity transform
     *
     * @return TRUE if the scale factors are unity
     */
    bool IsUnity( void )
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
class TRANSFORM
{
    TRANSLATION t;          ///< translation parameters
    ROTATION    r;          ///< rotation parameters
    SCALE       s;          ///< scaling parameters
    bool    unity;          ///< TRUE if the transform is a unity transform
    void testUnity( void ); ///< Sets the @var unity flag if the transform is unity

public:
    TRANSFORM();
    TRANSFORM( TRANSLATION T, ROTATION R, SCALE S );

    /**
     * Set all transform parameters
     *
     * @param T [in]
     * @param R [in]
     * @param S [in]
     */
    void Set( TRANSLATION T, ROTATION R, SCALE S );

    /**
     * Set the Translation parameter
     *
     * @param T [in]
     */
    void SetTranslation( TRANSLATION T );

    /**
     * Set the Translation parameter
     *
     * @param aTranslation [in]
     */
    void SetTranslation( QUAT aTranslation );

    /**
     * Set the Translation parameter
     *
     * @param x [in]
     * @param y [in]
     * @param z [in]
     */
    void SetTranslation( double x, double y, double z );

    /**
     * Set the rotation parameters
     *
     * @param R [in]
     */
    void SetRotation( ROTATION R );

    /**
     * Set the rotation parameters
     *
     * @param q [in]
     */
    void SetRotation( QUAT aRotation );

    /**
     * Set the rotation parameters
     *
     * @param angle [in]
     * @param x     [in]
     * @param y     [in]
     * @param z     [in]
     */
    void SetRotation( double angle, double x, double y, double z );

    /**
     * Set the scaling parameters
     *
     * @param S [in]
     */
    void SetScale( SCALE S );

    /**
     * Set the scaling parameters
     *
     * @param aScale [in]
     */
    void SetScale( QUAT aScale );

    /**
     * Set the scaling parameters
     *
     * @param x [in]
     * @param y [in]
     * @param z [in]
     */
    void SetScale( double x, double y, double z );

    /**
     * Set the scaling parameters
     *
     * @param n [in] Uniform scaling to apply to X Y Z axes
     */
    void SetScale( double n );

    /**
     * Transform the point represented by a quaternion
     *
     * @param aPoint [in,out]
     */
    void Transform( QUAT& aPoint );

    /**
     * Transform the point represented by discrete coordinates
     *
     * @param x [in,out]
     * @param y [in,out]
     * @param z [in,out]
     */
    void Transform( double& x, double& y, double& z );

    /**
     * Transform a set of points represented by quaternions
     *
     * @param aPointList [in,out] array of points
     * @param nPoints [in] number of points in the array @var aPointList
     */
    void Transform( QUAT* aPointList, int nPoints );

    /**
     * Transform a set of points represented by discrete coordinates
     *
     * @param x   [in,out]
     * @param y   [in,out]
     * @param z   [in,out]
     * @param np  [in] number of points to transform
     */
    void Transform( double* x, double* y, double* z, int np );

    /**
     * Retrieve the translation parameters
     *
     * @param extt [out]
     */
    void GetTranslation( TRANSLATION& aTranslation );

    /**
     * Retrieve the rotation parameters
     *
     * @param extr [out]
     */
    void GetRotation( ROTATION& aRotation );

    /**
     * Retrieve the scaling parameters
     *
     * @param exts [out]
     */
    void GetScale( SCALE& aScale );
};

}       // namespace KC3D

#endif  // TRANSFORM_H
