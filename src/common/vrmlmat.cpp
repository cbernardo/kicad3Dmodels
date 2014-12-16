/*
 *      file: vrmlmat.cpp
 *
 *      Copyright 2012-2014 Cirilo Bernardo (cjh.bernardo@gmail.com)
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


#include <iomanip>

#include <vrmlmat.h>

using namespace std;
using namespace KC3D;

int VRMLMAT::WriteMaterial( std::ofstream& aVRMLFile, int aTabDepth )
{
    if( aTabDepth < 0 )
        aTabDepth = 0;

    if( aTabDepth > 5 )
        aTabDepth = 5;

    string fmt( 4 * aTabDepth, ' ' );

    aVRMLFile << fmt << "material DEF " << material << " Material {\n";

    aVRMLFile << setprecision( 5 );
    aVRMLFile << fmt << "    diffuseColor ";
    aVRMLFile << diffuse[0] << " " << diffuse[1] << " " << diffuse[2] << "\n";

    aVRMLFile << fmt << "    emissiveColor ";
    aVRMLFile << emissive[0] << " " << emissive[1] << " " << emissive[2] << "\n";

    aVRMLFile << fmt << "    specularColor ";
    aVRMLFile << specular[0] << " " << specular[1] << " " << specular[2] << "\n";

    aVRMLFile << fmt << "    ambientIntensity " << ambient << "\n";

    aVRMLFile << fmt << "    transparency " << transparency << "\n";

    aVRMLFile << fmt << "    shininess " << shininess << "\n";

    aVRMLFile << fmt << "}\n";

    return aVRMLFile.good() ? 0 : -1;
}
