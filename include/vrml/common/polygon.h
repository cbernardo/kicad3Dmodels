/*
 *      file: polygon.h
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
 *
 *      This class represents a regular polygon. Override the default
 *      Paint() routine in any derived class which is not a regular polygon.
 *
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include <iosfwd>

namespace KC3D
{
class VRMLMAT;
class TRANSFORM;
class QUAT;

/**
 * \ingroup vrml_tools
 * This class represents a generic polygon and has
 * procedures implemented for rendering convex polygons.
 */
class POLYGON
{
private:
    POLYGON& operator=( const POLYGON& p );

protected:
    double* x, * y, * z;    // array of vertices after transformation
    int nv;                 // number of vertices (3 .. 360)
    bool valid;             // true if Calc has successfully completed

    virtual void init( void );

public:
    POLYGON();
    virtual ~POLYGON();

    /**
     * Test if the object holds a valid set of vertices
     *
     * @return TRUE if a valid vertex set is present
     */
    bool IsValid( void ) const;

    /**
     * \brief Create a duplicate instance of this type of POLYGON
     *
     * This method must be implemented by all derived classes to
     * provide a means of creating a copy of the derived class.
     *
     * @return Duplicate instance of *this
     */
    virtual POLYGON* Clone( void ) const = 0;

    /**
     * \brief Calculate the vertices of a polygon
     *
     * Each derived class must implement this method to calculate
     * a set of vertices representing the derived class.
     *
     * @param x [in] maximum extent of the polygon along the X axis
     * @param y [in] maximum extent of the polygon along the Y axis
     * @param t [in] local transform to apply to results
     * @return 0 for success, -1 for failure
     */
    virtual int Calc( double x, double y, TRANSFORM& t ) = 0;

    /**
     * \brief Render the face of a polygon
     *
     * Write out a VRML2.0 Shape block defining a rendered face of
     * a convex polygon. A derived class must override this method
     * if it implements a concave polygon.
     *
     * @param isCCW         [in] order of vertex enumeration; determines visible side
     * @param aTransform    [in] geometric transform to apply to output vertices
     * @param aMaterial     [in] VRMLMAT material appearance specification
     * @param reuseMaterial [in] set to true to reuse a previously written material name
     * @param aVRMLFile     [in] open output file
     * @param aTabDepth     [in] indent level for formatting
     * @return 0 for success, -1 for failure
     */
    virtual int Paint( bool isCCW, TRANSFORM& aTransform, VRMLMAT& aMaterial, bool reuseMaterial,
            std::ofstream& aVRMLFile, int aTabDepth = 0 );

    /**
     * \brief Render a surface defined by the perimeter of 2 polygons
     *
     * Write out a series of VRML2.0 Shape blocks defining a surface
     * described by the vertices of two polygons.  The polygons must
     * have the same number of vertices. As an example of its use,
     * two circles can be used to create a tubular or cylindrical
     * section as well as a disc with a hole in it.
     *
     * @param isCCW         [in] order of vertex enumeration; determines visible side
     * @param aPolygon      [in] the second polygon describing the edge of the surface
     * @param aTransform    [in] geometric transform to apply to output vertices
     * @param aMaterial     [in] VRMLMat material appearance
     * @param reuseMaterial [in] TRUE to reuse a previously written material name
     * @param aVRMLFile     [in] open output file
     * @param aTabDepth     [in] indent level for formatting
     * @return 0 for success, -1 for failure
     */
    virtual int Stitch( bool isCCW, POLYGON& aPolygon, TRANSFORM& aTransform, VRMLMAT& aMaterial,
                         bool reuseMaterial, std::ofstream& aVRMLFile, int aTabDepth );

    /**
     * \ Brief extrude a polygon and write to file
     *
     * Extrude this polygon using @param aTransform to calculate the orientation, position,
     * and size of the second polygon.
     *
     * @param aRenderCap0     [in] true to render the first end of the extrusion
     * @param aRenderCap1     [in] true to render the final end of the extrusion
     * @param isCCW           [in] true to render the outer surface visible
     * @param aStartPoint     [in] start point (0,0,0) coordinate of first polygon
     * @param aTransform      [in] transform to calculate the location of the second polygon
     * @param aFinalTransform [in] transform to apply to output
     * @param aMaterial       [in] appearance specification
     * @param reuseMaterial   [in] true to reuse @param aMaterial
     * @param aVRMLFile       [in] open output file
     * @param aTabDepth       [in] formatting indent level
     * @return 0 for success, -1 for failure
     */
    virtual int Extrude( bool aRenderCap0,
                         bool aRenderCap1,
                         bool isCCW,
                         QUAT& aStartPoint,
                         TRANSFORM& aTransform,
                         TRANSFORM& aFinalTransform,
                         VRMLMAT& aMaterial,
                         bool reuseMaterial,
                         std::ofstream& aVRMLFile,
                         int aTabDepth = 0 );

    /**
     * \brief Transform vertices
     *
     * Transform all the vertices in a polygon. Unlike the transform parameters
     * supplied to the routines Paint and Stitch which only affects the output,
     * this transform alters the object's internally stored vertices.
     *
     * @param T [in] geometric transform to apply to internal vertex list
     * @return 0 for success, -1 for failure
     */
    int Xform( TRANSFORM& T );

    /**
     * \brief Retrieve the internal vertex list
     * @param px [out] pointer to internal array of X coordinates
     * @param py [out] pointer to internal array of Y coordinates
     * @param pz [out] pointer to internal array of Z coordinates
     * @return number of vertices in the list or -1 for failure
     */
    int GetVertices( double** px, double** py, double** pz ) const;

    /**
     * \brief Return the number of internal vertices
     */
    int GetNVertices( void )
    {
        return nv;
    }

    /**
     * \brief Return the indexed X coordinate.
     * If the index is out of bounds 0.0 is returned; it is up to
     * the user to ensure that the index is within bounds
     */
    double GetX( int aIndex )
    {
        if( aIndex < 0 || aIndex >= nv )
            return 0.0;

        return x[aIndex];
    }

    /**
     * \brief Return the indexed Y coordinate.
     * If the index is out of bounds 0.0 is returned; it is up to
     * the user to ensure that the index is within bounds
     */
    double GetY( int aIndex )
    {
        if( aIndex < 0 || aIndex >= nv )
            return 0.0;

        return y[aIndex];
    }

    /**
     * \brief Return the indexed Y coordinate.
     * If the index is out of bounds 0.0 is returned; it is up to
     * the user to ensure that the index is within bounds
     */
    double GetZ( int aIndex )
    {
        if( aIndex < 0 || aIndex >= nv )
            return 0.0;

        return z[aIndex];
    }

    /**
     * \brief Reverse the internal vertex list
     * @return true if the order was reversed; false if no points to reverse
     */
    bool Reverse( void );

};


/**
 * \ingroup vrml_tools
 * @class FAKEPOLY implements a POLYGON which does not own its
 * vertices. It is useful where transient points are treated as
 * vertices of a polygon.
 */
class FAKEPOLY : public POLYGON
{
public:
    virtual ~FAKEPOLY();

    // inherited methods
    POLYGON*    Clone( void ) const;
    int         Calc( double xl, double yl, TRANSFORM& t );

    // extended methods
    void SetParams( double* x, double* y, double* z, int np, bool valid );
};


/**
 * \ingroup vrml_tools
 * @class SIMPLEPOLY implements a POLYGON which owns but does not calculate
 * its own vertices or support a clone(). The vertices are copied from any
 * other polygon and may be transformed, painted and stitched. This class
 * is intended to aid where multiple transformations on a copy of a polygon
 * are required.
 */
class SIMPLEPOLY : public POLYGON
{
    int memReserved;    // memory reserved for vertex list

public:
    SIMPLEPOLY();
    SIMPLEPOLY( const SIMPLEPOLY& aSimplePoly );
    SIMPLEPOLY( const POLYGON& aPolygon );
    ~SIMPLEPOLY();

    // inherited methods
    POLYGON*    Clone( void ) const;
    int         Calc( double xFactor, double yFactor, TRANSFORM& aTransform );

    // extended methods
    int SetValues( const POLYGON& aPolygon );
    int AddVertex( double X, double Y, double Z );
};
}    // namespace kc3d

#endif /* POLYGON_H_ */
