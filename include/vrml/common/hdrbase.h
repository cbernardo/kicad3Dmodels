/*
 *      file: hdrbase.h
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
 *
 *      This class represents a rectangular polygon which can be used to generate
 *      rectangular leads or pins.
 *
 */

/*
 * XXX:
 * To Add:
 *  'hdy' to control Y dimension of square pin on bottom
 *  'pbev' to control bevel of hole (male = bottom + top, female = bottom only)
 *  'fbev' to control bevel of top hole for square female case
 */
#ifndef HDRBASE_H_
#define HDRBASE_H_

#include "polygon.h"

namespace kc3d {

/**
 * \ingroup vrml_tools
 * This class represents a PCB header base; the base
 * may be plain or beveled and have either circular or
 * rectangular holes at each end.
 */
class Hdrbase: private kc3d::Polygon
{
private:    
    // Calculate the vertices representing the base; parameters
    // must have been previously set via SetParams.
    int calc(void);
    
    /**
     * \brief Render the top and bottom of the header base
     *
     * Write out a VRML2.0 Shape block defining the top and
     * bottom of the header block.
     *
     * @param t     [in] geometric transform to apply to output vertices
     * @param color [in] VRMLMat material appearance of header body
     * @param reuse_color [in] set to TRUE to reuse a previously written material name
     * @param fp    [in] open output file
     * @param tabs  [in] indent level for formatting
     * @return 0 for success, -1 for failure
     */
    int paint(kc3d::Transform &t, kc3d::VRMLMat &color, bool reuse_color,
            std::ofstream &fp, int tabs = 0);

    /**
     * \brief Render the sides of the header
     *
     * Write out a series of VRML2.0 Shape blocks defining the sides
     * of the header block.
     *
     * @param t     [in] geometric transform to apply to output vertices
     * @param color [in] VRMLMat material appearance
     * @param reuse_color [in] TRUE to reuse a previously written material name
     * @param fp    [in] open output file
     * @param tabs  [in] indent level for formatting
     * @return 0 for success, -1 for failure
     */
    int stitch(kc3d::Transform &t, kc3d::VRMLMat &color, bool reuse_color,
            std::ofstream &fp, int tabs = 0);

    // overridden and hidden class methods
    // the overridden Paint prints an error message and returns
    int Paint(bool ccw, kc3d::Transform &t, kc3d::VRMLMat &color, bool reuse_color,
            std::ofstream &fp, int tabs = 0);

    // the overridden Stitch prints an error message and returns
    int Stitch(bool ccw, Polygon &p2, kc3d::Transform &t,
            kc3d::VRMLMat &color, bool reuse_color, std::ofstream &fp, int tabs = 0);

    // the overridden Calc prints an error message and returns
    int Calc(double, double, kc3d::Transform&);

protected:
    double bev;     ///< the length of the bevel
    double height;  ///< total height of the header
    int xpins;      ///< number of columns of pins
    int ypins;      ///< number of rows of pins
    bool squarebot; ///< TRUE if we're using square holes on the bottom
    bool squaretop; ///< TRUE if we're using square holes on the bottom
    double hd0;     ///< hole dimension (edge or diameter), bottom
    double hdy;     ///< hole y dimension (bottom, or top+bottom for M case)
    double hd1;     ///< hole dimension, top, large aperture
    double sh;      ///< height of the shoulder (<= 0.0 for none)
    bool hassh;     ///< true if the shoulder is to be rendered, otherwise 'sh' is an empty space
    double xpitch;
    double ypitch;
    int ns;
    bool male;      ///< TRUE if the case is for a male connector
    double pbev;    ///< bevel for the bottom hole (and top hole for M case)
    double fbev;    ///< bevel for the top hole (only for F case)

    // vertices
    double *x, *y, *z;  // vertices for body
    int nv;             // number of vertices (6*columns + 2); NOTE: not the total points
    double sv[3][8];    // vertices for shoulders (always 4*2)
  
    void setDefaults(void);
    int makeHoles1(kc3d::Transform &t, kc3d::VRMLMat &color, bool reuse_color,
            std::ofstream &fp, int tabs = 0);
    int makeHoles2(kc3d::Transform &t, kc3d::VRMLMat &color, bool reuse_color,
            std::ofstream &fp, int tabs = 0);
    int makeHoles3(kc3d::Transform &t, kc3d::VRMLMat &color, bool reuse_color,
            std::ofstream &fp, int tabs = 0);

public:
    Hdrbase();
    Hdrbase(double bevel);
    Hdrbase(const Hdrbase &p);
    virtual ~Hdrbase();

    Hdrbase &operator=(const Hdrbase &p);

    /**
     * Create a duplicate instance of this Hdrbase
     *
     * @return Duplicate of *this
     */
    Polygon *clone(void) const;




    /**
     * \brief Set the object's parameters
     *
     * @param xpitch [in] pin pitch for columns
     * @param ypitch [in] pin pitch for rows
     * @param bevel [in] bevel between columns
     * @param height [in] overall height of case
     * @param sh [in] shoulder height (<=0 for none)
     * @param hd0 [in] dimension for bottom holes
     * @param hd1 [in] dimension for top holes
     * @param square [in] TRUE if holes are square (false = circular)
     * @param columns [in] number of columns (>= 2)
     * @param rows [in] number of rows (>= 1)
     * @param ns [in] number of vertices (applies to round holes only)
     * @return 0 for success, -1 for failure
     */
    int SetParams(double xpitch, double ypitch, double bevel, double height,
            double sh, bool hassh, double hd0, double hdy, double hd1,
            bool squarebot, bool squaretop, bool male, double pbev, double fbev,
            int columns, int rows, int ns);

    /**
     * \brief Write the header shape to an output file
     *
     * @param t     [in] geometric transform to apply to output vertices
     * @param color [in] VRMLMat material appearance
     * @param reuse_color [in] TRUE to reuse a previously written material name
     * @param fp    [in] open output file
     * @param tabs  [in] indent level for formatting
     * @return 0 for success, -1 for failure
     */
    int Build(kc3d::Transform &t, kc3d::VRMLMat &color, bool reuse_color,
            std::ofstream &fp, int tabs = 0);
};

}   // namespace kc3d

#endif /* HDRBASE_H_ */
