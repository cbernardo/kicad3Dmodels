/*
 *      file: vrmlmat.h
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


#ifndef VRMLMAT_H
#define VRMLMAT_H

#include <fstream>

#include <material.h>

namespace KC3D
{
/**
 * \file
 * \defgroup vrml_tools VRML Tools
 */

/**
 * \ingroup vrml_tools
 * \brief VRML2.0 Material Specification
 *
 * This class derives from the Material class and
 * implements a procedure to write the information
 * to a VRML2.0 output file
 */
class VRMLMAT : public MATERIAL
{
public:
    /**
     * Write out a VRML2.0 compliant material block
     *
     * @param aVRMLFile [in]    output file
     * @param aTabDepth [in]    indentation depth of the text block
     * @return 0 for success, -1 for failure
     */
    int WriteMaterial( std::ofstream& aVRMLFile, int aTabDepth = 0 );
};

}    // namespace KC3D

#endif // VRMLMAT_H
