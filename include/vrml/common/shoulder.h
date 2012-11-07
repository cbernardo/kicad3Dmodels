/*
 *      file: shoulder.h
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
 *      This class represents a shoulder such as the one found on the
 *      bottom of a SIL header which creates a small gap between the
 *      component and the board.
 *
 *      _______o_______ l = length
 *      \             / h = height
 *       \___________/  w = width
 *                      t = taper (radians)
 *                      r = radius of inner lip
 */

#ifndef SHOULDER_H_
#define SHOULDER_H_

namespace kc3d {

class Transform;
class VRMLMat;

/**
 * \ingroup vrml_tools
 * This class represents the ridge found on the
 * bottom of some components such as SIL headers.
 */
class Shoulder
{
private:
    bool valid;     ///< TRUE if the class holds a valid set of parameters
    double p0[3][7], p1[3][7];  ///< 2 polygons defining the shoulder
    int np; ///< number of points defining the shoulder

public:
    Shoulder();
    virtual ~Shoulder();

    /**
     * Calculate the outline of the shoulder.
     *
     * @param l [in] length of top part
     * @param h [in] height of the shoulder
     * @param w [in] depth of the shoulder
     * @param t [in] taper angle (radians) of the ends of the shoulder
     * @param r [in] radius of the lower inner edge of the shoulder
     * @param tx [in] local transformation to apply to results
     * @return 0 for success, -1 for failure
     */
    int Calc(double l, double h, double w, double t, double r, Transform &tx);

    /**
     * \brief Write out shoulder data
     *
     * Write out multiple VRML2.0 Shape blocks to define
     * the shoulder. The user must have made a previous
     * call to SetupXForm to create the encompassing Transform block.
     *
     * @param t     [in] transform to apply to the output coordinates
     * @param color [in] VRMLMat material appearance specification
     * @param reuse [in] TRUE to reuse a previously written material name
     * @param fp    [in] open output file
     * @param tabs  [in] indent level for formatting
     * @return 0 for success, -1 for failure
     */
    int Build(Transform &t, VRMLMat &color, bool reuse,
            std::ofstream &fp, int tabs = 0);
};

}   // namespace kc3d

#endif /* SHOULDER_H_ */
