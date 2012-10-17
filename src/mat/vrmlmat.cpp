/*
 *      file: vrmlmat.cpp
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


#include <iomanip>

#include "vrmlmat.h"

using namespace std;

int VRMLMat::WriteMaterial(std::ofstream &file, int tabs)
{
    if (tabs < 0) tabs = 0;
    if (tabs > 5) tabs = 5;
    string fmt(4*tabs, ' ');

    file << fmt << "material DEF " << material << " Material {\n";
    file << fmt << "    diffuseColor " << setprecision(2) << diffuse[0] <<
            " " << diffuse[1] << " " << diffuse[2] << "\n";
    file << fmt << "    emissiveColor " << setprecision(2) << emissive[0] <<
            " " << emissive[1] << " " << emissive[2] << "\n";
    file << fmt << "    specularColor " << setprecision(2) << specular[0] <<
            " " << specular[1] << " " << specular[2] << "\n";
    file << fmt << "    ambientIntensity " << setprecision(2) << ambient << "\n";
    file << fmt << "    transparency " << setprecision(2) << transparency << "\n";
    file << fmt << "    shininess " << setprecision(2) << shininess << "\n";
    file << fmt << "}\n";
    return file.good() ? 0 : -1;
}
