/*
 *      file: genhdr.h
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

/*
 * This model represents a generic PCB header
 * Parameters control:
 *      square or round pins
 *      male or female
 *      height
 *      shoulder height
 *      bevel
 *      xpitch
 *      ypitch
 *      columns
 *      rows (supports > 2 rows although this is probably not needed)
 *      hole dimension
 *      second hole dimension for round female headers
 *
 *      body color
 *      pin color
 *      funnel color for round pins
 *
 */


#ifndef GENHDR_H
#define GENHDR_H

#include <iosfwd>

#include <vrmlmat.h>
#include <transform.h>

namespace KC3DCONN
{

/**
 * \ingroup vrml_tools
 * \brief GENHDR generic rectangular header
 *
 * This class represents a generic thru-hole rectangular header
 */
class GENHDR
{
private:
    KC3D::VRMLMAT bcolor;   // body (casing) color
    KC3D::VRMLMAT pcolor;   // pin color
    KC3D::VRMLMAT fcolor;   // funnel color (for female hdr only)
    KC3D::VRMLMAT scolor;   // shroud color (for female hdr, circular funnel only)

    int cols;               // columns; must be >= 2
    int rows;               // rows; must be >= 1
    double xp;              // pitch for columns
    double yp;              // pitch for rows
    double sh;              // shoulder height
    double bh;              // body height
    double bev;             // bevel
    bool hassh;             // true if we wish to render a shoulder

    bool squarebot;         // true for square bottom pins
    bool squaretop;         // true for square top pins (for M connector, must be same as squarebot)
    bool male;              // true if male header
    double pbev;            // pin bevel (square pins only)
    double fbev;            // funnel bevel (female square pins only)
    double pd;              // pin depth (below top of board)
    double pl;              // pin length (primarily for male pin)
    double pd0;             // pin diameter 0 (or length of side)
    double pdy;             // pin dimension in Y direction (square pins only)
    double pd1;             // pin diameter 1 (Female, pin dia. within header)
    double pd2;             // pin diameter 2 (Female, dia. of funnel)
    double pd3;             // pin diameter, round pin, lower part
    double ftc;             // funnel taper coefficient (nominal 1.1)
    double pt;              // pin taper
    double pts;             // pin taper ratio
    int ns;                 // number of sides to a circular pin (must be 4*N, 4 .. 360)
    double fd;              // funnel depth (for female headers)

    // book keeping
    bool hasColors;     // true when colors have been loaded
    bool hasBparams;    // true when body parameters have been set
    bool hasPparams;    // true when pin parameters have been set

    // render the case
    int makeCase( KC3D::TRANSFORM& t, std::ofstream& fp, int tabs = 0 );

    // render the pins
    int makePins( KC3D::TRANSFORM& t, std::ofstream& fp, int tabs = 0 );

    // render shrouds for circular pins in female headers
    int makeShrouds( KC3D::TRANSFORM& t, std::ofstream& fp, int tabs = 0 );

    // render funnels for female headers
    int makeFunnels( KC3D::TRANSFORM& t, std::ofstream& fp, int tabs = 0 );

public:
    GENHDR();

    /**
     * Create the VRML object and write it to a file
     *
     * @param aTransform [in] geometric transform to apply to output
     * @param aPartName [in] name of the part as used in the VRML file
     * @param aVRMLFile [in] open VRMML output file
     * @param aTabDepth [in] indentation level for output
     * @return 0 for success, -1 for failure
     */
    int Build( KC3D::TRANSFORM& aTransform, std::string aPartName,
               std::ofstream& aVRMLFile, int aTabDepth = 0 );

    /**
     * Read VRML material appearances from file
     *
     * @param aBodyMatFileName [in] path to VRML appearance for body
     * @param aPinMatFileName [in] path to VRML appearance for pin
     * @param aFunnelMatFileName [in] path to VRML appearance for funnel
     * @param aShroudMatFileName [in] path to VRML appearance for shroud
     * @return 0 for success, -1 for failure
     */
    int SetColors( std::string aBodyMatFileName, std::string aPinMatFileName,
                   std::string aFunnelMatFileName, std::string aShroudMatFileName );

    /**
     * Set header case parameters
     *
     * @param nCols [in] number of columns in header (min: 1)
     * @param nRows [in] number of rows in header (min: 1)
     * @param aColPitch [in] column pitch
     * @param aRowPitch [in] row pitch
     * @param aHeight [in] total height of case
     * @param aShoulder [in] height of shoulder (< 0 for no shoulder)
     * @param hasShoulder [in] true if a shoulder is to be rendered
     * @param aBevel [in] length of bevel in between pins
     * @return 0 for success, -1 for failure
     */
    int SetCase( int nCols, int nRows, double aColPitch, double aRowPitch,
                 double aHeight, double aShoulder, bool hasShoulder, double aBevel );

    /**
     * Set header pin parameters
     *
     * @param isBotSquare [in] true if the bottom part of the pin is square/rectangular
     * @param isTopSquare [in] true if the top part of the pin is square/rectangular
     * @param isMale [in] true if the header is male
     * @param aPinBevel [in] > 0 if the (rectangular) pin is beveled
     * @param aFunnelBevel [in] > 0 if the funnel is beveled
     * @param aPinDepth [in] legth of the pin below the PCB top
     * @param aPinLength [in] total legth of the pin
     * @param aBotPinXSize [in] X dimension of the bottom part of the pin
     * @param aBotPinXSize [in] Y dimension of the bottom part of the pin
     * @param aSocketSize [in] size/diameter of receptacle inside the header case
     * @param aFunnelSize [in] size/diameter of funnel
     * @param aPinOuterSize [in] outer diameter of a pin boss at either end of case
     * @param aFunnelTaper [in] taper coefficient of funnel (nominally 1.1)
     * @param aPinTaperLength [in] length of taper
     * @param aPinTaperSize [in] size/dia of small end of taper
     * @param aNSides [in] number of sides on the pin
     * @param aSocketDepth [in] total depth of the receptacle
     * @return 0 for success, -1 for failure
     */
    int SetPins( bool isBotSquare, bool isTopSquare, bool isMale, double aPinBevel,
                 double aFunnelBevel, double aPinDepth, double aPinLength,
                 double aBotPinXSize, double aBotPinYSize, double aSocketSize,
                 double aFunnelSize, double aPinOuterSize, double aFunnelTaper,
                 double aPinTaperLength, double aPinTaperSize, int aNSides,
                 double aSocketDepth );
};

}    // namespace KC3DCONN

#endif // GENHDR_H
