/*
 *      file: pin.h
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

#ifndef PIN_H
#define PIN_H

#include <iosfwd>

namespace KC3D
{
class POLYGON;
class POLYRECT;
class VRMLMAT;
class TRANSFORM;

/**
 * \ingroup vrml_tools
 * Parameters describing a rectangular or elliptical wire or pin
 */
struct PPARAMS
{
    double  w;      ///< pin width
    double  d;      ///< pin depth
    double  tap;    ///< pin taper, < 0 to indicate no taper
    double  stw;    ///< scale factor for taper in 'w' dimension
    double  std;    ///< scale factor for taper in 'd' dimension
    bool    dbltap; ///< true if tapered at both ends
    double  h;      ///< pin height (straight vertical section)
    double  r;      ///< pin bend radius, < 0 to indicate no bend (and no hz section)
    int nb;         ///< number of segments in a bend (default 5)
    double  l;      ///< length of straight horizontal section, < 0 for no hz section
    double  bev;    ///< bevel (applicable to rectangular pins only); < 0 means no bevel
    double  bend;   ///< bend angle (radians)
    int ns;         ///< number of sides for a circular pin (default 24)

    PPARAMS();
};


/**
 * \ingroup vrml_tools
 * This class represent a wire or pin. The nominal orientation
 * is vertical with an optional bend of 0..PI degrees towards the
 * right (+X axis) and an optional second straight segment.
 * The pin may have a rectangular or an elliptical cross-section.
 * A rectangular section may be beveled and an elliptical section
 * may have 3..360 vertices.
 */
class PIN
{
private:
    POLYGON** poly; ///< pointer to polygons (rectangles or circles)
    int nr;         ///< number of polygons
    bool    valid;  ///< true when we have valid polygons
    bool    square; ///< true (default) for square pins

    void cleanup( void );

protected:
    PPARAMS pin;

public:
    PIN();
    PIN( const PIN& aPin );
    virtual ~PIN();

    PIN& operator=( const PIN& aPin );

    /**
     * \brief Calculate intermediate polygons
     *
     * This method calculates the position and orientation of
     * polygons which describe the boundaries of the pin. The
     * pin may have a rectangular (default) cross-section or
     * an elliptical cross-section. The pin shape may be set
     * by invoking SetShape prior to invoking this method.
     *
     * @param pp [in] parameters controlling the shape of the pin
     * @param t  [in] geometric transformation to apply to the results
     * @return 0 for success, -1 for failure
     */
    virtual int Calc( const PPARAMS& aPinParam, TRANSFORM& aTransform );

    /**
     * \brief Write out pin shape information
     *
     * Write a series of VRML2.0 Shape blocks describing the
     *
     * @param aRenderCap0 [in] TRUE if the first end face of the pin is to be rendered
     * @param aRenderCap1 [in] TRUE if the final end face of the pin is to be rendered
     * @param aFinalTransform [in] geometric transform to apply to output vertices
     * @param aMaterial   [in] VRMLMAT material appearance
     * @param reuseMaterial [in] TRUE if a previously written material is to be used
     * @param aVRMLFile     [in] open VRML file
     * @param aTabDepth     [in] indent level for formatting
     * @return
     */
    virtual int Build( bool aRenderCap0, bool aRenderCap1, TRANSFORM& aFinalTransform,
                       VRMLMAT& aMaterial, bool reuseMaterial,
                       std::ofstream& aVRMLFile, int aTabDepth = 0 );

    /**
     * Set the shape of a generic pin or wire
     *
     * @param isSquare [in] TRUE for a rectangular cross section, FALSE for an elliptical section
     */
    void SetShape( bool isSquare );
};

}    // namespace KC3D

#endif // PIN_H
