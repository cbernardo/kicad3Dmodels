/*
 *      file: dimple.h
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
 *      This class creates a faceted bump based on a given Circle and bump depth.
 *      The depth must be ( >0, < radius*2) or ( <0, > -radius*2)
 *
 */

#ifndef DIMPLE_H
#define DIMPLE_H

#include <iostream>

#include <circle.h>


namespace KC3D
{
    class TRANSFORM;
    class VRMLMAT;

    class DIMPLE
    {
    private:
        bool paramSet;
        CIRCLE bump;
        double refAngle;
        double dAngle;
        double factor;
        double depth;
        int sides;
        int sections;

        bool writeVertices( bool isCapped, TRANSFORM& aTransform,
                            std::ofstream& aVRMLFile, int aTabDepth );

        bool writeIndices( bool isCapped, bool isCCW, VRMLMAT& aMaterial, bool reuseMaterial,
                           std::ofstream& aVRMLFile, int aTabDepth );

    public:
        DIMPLE();

        bool SetParams( double aRadius, double aDepth, int aNSides, int aNSections );

        bool WriteVRML( bool isCapped, bool isCCW, TRANSFORM& aTransform, VRMLMAT& aMaterial,
                        bool reuseMaterial, std::ofstream& aVRMLFile, int aTabDepth );

    };

}

#endif // DIMPLE_H