/*
 *      file: shoulder.h
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

#ifndef SHOULDER_H
#define SHOULDER_H

namespace KC3D
{
class TRANSFORM;
class VRMLMAT;

/**
 * \ingroup vrml_tools
 * This class represents the ridge found on the
 * bottom of some components such as SIL headers.
 */
class SHOULDER
{
private:
    bool valid;                 ///< TRUE if the class holds a valid set of parameters
    double p0[3][7], p1[3][7];  ///< 2 polygons defining the shoulder
    int np;                     ///< number of points defining the shoulder

public:
    SHOULDER();
    virtual ~SHOULDER();

    /**
     * Calculate the outline of the shoulder.
     *
     * @param aLength    [in] length of top part
     * @param aHeight    [in] height of the shoulder
     * @param aWidth     [in] depth of the shoulder
     * @param aTaper     [in] taper angle (radians) of the ends of the shoulder
     * @param aRadius    [in] radius of the lower inner edge of the shoulder
     * @param aTransform [in] local transformation to apply to results
     * @return 0 for success, -1 for failure
     */
    int Calc( double aLength, double aHeight, double aWidth, double aTaper,
              double aRadius, TRANSFORM& aTransform );

    /**
     * \brief Write out shoulder data
     *
     * Write out multiple VRML2.0 Shape blocks to define
     * the shoulder. The user must have made a previous
     * call to SetupXForm to create the encompassing Transform block.
     *
     * @param aTransform    [in] transform to apply to the output coordinates
     * @param aMaterial     [in] VRMLMat material appearance specification
     * @param reuseMaterial [in] TRUE to reuse a previously written material name
     * @param aVRMLFile     [in] open output file
     * @param aTabDepth     [in] indent level for formatting
     * @return 0 for success, -1 for failure
     */
    int Build( TRANSFORM& aTransform, VRMLMAT& aMaterial, bool reuseMaterial,
               std::ofstream& aVRMLFile, int aTabDepth = 0 );
};


}    // namespace KC3D

#endif // SHOULDER_H
