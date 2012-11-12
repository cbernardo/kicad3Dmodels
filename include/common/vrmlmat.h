/*
 *      file: vrmlmat.h
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


#ifndef VRMLMAT_H_
#define VRMLMAT_H_

#include <fstream>

#include "material.h"

namespace kc3d {

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
class VRMLMat : public Material
{
public:
    /**
     * Write out a VRML2.0 compliant material block
     *
     * @param file  [in]    output file
     * @param tabs  [in]    indentation depth of the text block
     * @return 0 for success, -1 for failure
     */
    int WriteMaterial(std::ofstream &file, int tabs = 0, bool mainblock = false);
};

}   // namespace kc3d

#endif /* VRMLMAT_H_ */
