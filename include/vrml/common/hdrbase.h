/*
 *      file: hdrbase.h
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
 *      This class represents a rectangular polygon which can be used to generate
 *      rectangular leads or pins.
 *
 */

#ifndef HDRBASE_H
#define HDRBASE_H

#include <polygon.h>

namespace KC3D
{
/**
 * \ingroup vrml_tools
 * This class represents a PCB header base; the base
 * may be plain or beveled and have either circular or
 * rectangular holes at each end.
 */
class HDRBASE : private KC3D::POLYGON
{
private:
    // Calculate the vertices representing the base; parameters
    // must have been previously set via SetParams.
    int calc( void );

    /**
     * \brief Render the top and bottom of the header base
     *
     * Write out a VRML2.0 Shape block defining the top and
     * bottom of the header block.
     *
     * @param t     [in] geometric transform to apply to output vertices
     * @param color [in] VRMLMAT material appearance of header body
     * @param reuse_color [in] set to TRUE to reuse a previously written material name
     * @param fp    [in] open output file
     * @param tabs  [in] indent level for formatting
     * @return 0 for success, -1 for failure
     */
    int paint( KC3D::TRANSFORM& t, KC3D::VRMLMAT& color, bool reuse_color,
            std::ofstream& fp, int tabs = 0 );

    /**
     * \brief Render the sides of the header
     *
     * Write out a series of VRML2.0 Shape blocks defining the sides
     * of the header block.
     *
     * @param t     [in] geometric transform to apply to output vertices
     * @param color [in] VRMLMAT material appearance
     * @param reuse_color [in] TRUE to reuse a previously written material name
     * @param fp    [in] open output file
     * @param tabs  [in] indent level for formatting
     * @return 0 for success, -1 for failure
     */
    int stitch( KC3D::TRANSFORM& t, KC3D::VRMLMAT& color, bool reuse_color,
            std::ofstream& fp, int tabs = 0 );

    // overridden and hidden class methods
    // the overridden Paint prints an error message and returns
    int Paint( bool ccw, KC3D::TRANSFORM& t, KC3D::VRMLMAT& color, bool reuse_color,
            std::ofstream& fp, int tabs = 0 );

    // the overridden Stitch prints an error message and returns
    int Stitch( bool ccw, POLYGON& p2, KC3D::TRANSFORM& t,
            KC3D::VRMLMAT& color, bool reuse_color, std::ofstream& fp, int tabs = 0 );

    // the overridden Calc prints an error message and returns
    int Calc( double, double, KC3D::TRANSFORM& );

protected:
    double  bev;        ///< the length of the bevel
    double  height;     ///< total height of the header
    int xpins;          ///< number of columns of pins
    int ypins;          ///< number of rows of pins
    bool    squarebot;  ///< TRUE if we're using square holes on the bottom
    bool    squaretop;  ///< TRUE if we're using square holes on the bottom
    double  hd0;        ///< hole dimension (edge or diameter), bottom
    double  hdy;        ///< hole y dimension (bottom, or top+bottom for M case)
    double  hd1;        ///< hole dimension, top, large aperture
    double  sh;         ///< height of the shoulder (<= 0.0 for none)
    bool    hassh;      ///< true if the shoulder is to be rendered, otherwise 'sh' is an empty space
    double  xpitch;
    double  ypitch;
    int ns;
    bool male;      ///< TRUE if the case is for a male connector
    double  pbev;   ///< bevel for the bottom hole (and top hole for M case)
    double  fbev;   ///< bevel for the top hole (only for F case)

    // vertices
    double* x, * y, * z;    // vertices for body
    int nv;                 // number of vertices (6*columns + 2); NOTE: not the total points
    double sv[3][8];        // vertices for shoulders (always 4*2)

    void setDefaults( void );
    int makeHoles1( KC3D::TRANSFORM& t, KC3D::VRMLMAT& color, bool reuse_color,
            std::ofstream& fp, int tabs = 0 );
    int makeHoles2( KC3D::TRANSFORM& t, KC3D::VRMLMAT& color, bool reuse_color,
            std::ofstream& fp, int tabs = 0 );
    int makeHoles3( KC3D::TRANSFORM& t, KC3D::VRMLMAT& color, bool reuse_color,
            std::ofstream& fp, int tabs = 0 );

public:
    HDRBASE();
    HDRBASE( double bevel );
    HDRBASE( const HDRBASE& p );
    virtual ~HDRBASE();

    HDRBASE& operator=( const HDRBASE& p );

    /**
     * Create a duplicate instance of this HDRBASE
     *
     * @return Duplicate of *this
     */
    POLYGON* Clone( void ) const;

    /**
     * \brief Set the object's parameters
     *
     * @param aXpitch [in] pin pitch for columns
     * @param aYpitch [in] pin pitch for rows
     * @param aBevel [in] bevel between columns
     * @param aHeight [in] overall height of case
     * @param aShoulderHeight [in] shoulder height (<=0 for none)
     * @param aRenderShoulder [in] true if shoulder is to be rendered
     * @param aBotHoleXDim [in] X dimension for bottom holes
     * @param aBotHoleYDim [in] Y dimension for non-circular bottom holes
     * @param aTopHoleXDim [in] dimension for top holes
     * @param isSquareBot [in] true if bottom holes are square (false = circular)
     * @param isSquareTop [in] true if top holes are square (false = circular)
     * @param isMale [in] true if the header is male
     * @param aBotHoleBev [in] true if the bottom hole is beveled
     * @param aTopHoleBev [in] true if the top hole is beveled
     * @param aNCols [in] number of columns (>= 1)
     * @param aNRows [in] number of rows (>= 1)
     * @param aNVertex [in] number of vertices (applies to round holes only)
     * @return 0 for success, -1 for failure
     */
    int SetParams( double aXPitch, double aYPitch, double aBevel, double aHeight,
                   double aShoulderHeight, bool aRenderShoulder,
                   double aBotHoleXDim, double aBotHoleYDim, double aTopHoleXDim,
                   bool isSquareBot, bool isSquareTop, bool isMale,
                   double aBotHoleBev, double aTopHoleBev,
                   int aNCols, int aNRows, int aNVertex );

    /**
     * \brief Write the header shape to an output file
     *
     * @param aTransform [in] geometric transform to apply to output vertices
     * @param aMaterial [in] VRMLMAT material appearance
     * @param reuseMaterial [in] TRUE to reuse a previously written material name
     * @param aVRMLFile [in] open output file
     * @param aTabDepth [in] indent level for formatting
     * @return 0 for success, -1 for failure
     */
    int Build( KC3D::TRANSFORM& aTransform, KC3D::VRMLMAT& aMaterial, bool reuseMaterial,
               std::ofstream& aVRMLFile, int aTabDepth = 0 );
};

}    // namespace KC3D

#endif // HDRBASE_H
