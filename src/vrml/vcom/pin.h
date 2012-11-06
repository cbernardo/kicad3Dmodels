/*
 *      file: pin.h
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

#ifndef PIN_H_
#define PIN_H_

#include <iosfwd>

class Polygon;
class PolyRect;
class VRMLMat;
class Transform;

/**
 * Parameters describing a rectangular or elliptical wire or pin
 */
struct PParams
{
    double w;       ///< pin width
    double d;       ///< pin depth
    double tap;     ///< pin taper, < 0 to indicate no taper
    double stw;     ///< scale factor for taper in 'w' dimension
    double std;     ///< scale factor for taper in 'd' dimension
    bool   dbltap;  ///< true if tapered at both ends
    double h;       ///< pin height (straight vertical section)
    double r;       ///< pin bend radius, < 0 to indicate no bend (and no hz section)
    int    nb;      ///< number of segments in a bend (default 5)
    double l;       ///< length of straight horizontal section, < 0 for no hz section
    double bev;     ///< bevel (applicable to rectangular pins only); < 0 means no bevel
    double bend;    ///< bend angle (radians)
    int    ns;      ///< number of sides for a circular pin (default 24)

    PParams();
};

class Pin
{
private:
    Polygon **poly; ///< pointer to polygons (rectangles or circles)
    int nr;         ///< number of polygons
    bool valid;     ///< true when we have valid polygons
    bool square;    ///< true (default) for square pins

    void cleanup(void);

protected:
    PParams pin;

public:
    Pin();
    Pin (const Pin &p);
    virtual ~Pin();

    Pin &operator=(const Pin &p);

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
    virtual int Calc(const PParams &pp, Transform &t);

    /**
     * \brief Write out pin shape information
     *
     * Write a series of VRML2.0 Shape blocks describing the
     *
     * @param cap0  [in] TRUE if the first end face of the pin is to be rendered
     * @param cap1  [in] TRUE if the final end face of the pin is to be rendered
     * @param t     [in] geometric transform to apply to output vertices
     * @param color [in] VRMLMat material appearance
     * @param reuse_color [in] TRUE if a previously written color name is to be used
     * @param fp    [in] open output file
     * @param tabs  [in] indent level for formatting
     * @return
     */
    virtual int Build(bool cap0, bool cap1, Transform &t, VRMLMat &color, bool reuse_color,
            std::ofstream &fp, int tabs = 0);

    /**
     * Set the shape of a generic pin or wire
     *
     * @param square [in] TRUE for a rectangular cross section, FALSE for an elliptical section
     */
    void SetShape(bool square);
};

#endif /* PIN_H_ */
