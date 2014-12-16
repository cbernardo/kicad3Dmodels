/*
 *      file: vdefs.h
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
 *      #defines commonly used by the VRML tools
 */

/**
 * \file
 * \ingroup common_tools
 * \brief Macros common to K3D tools
 */

#ifndef VDEFS_H
#define VDEFS_H

#include <iostream>
#include <wx/string.h>

/**
 * \ingroup common_tools
 * Macro to dump file, line, function information to stderr
 */
#define ERRBLURB std::cerr << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "(): "

#endif // VDEFS_H
