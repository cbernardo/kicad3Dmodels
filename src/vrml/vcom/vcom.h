/*
 *      file: vcom.h
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
 *      Global routines frequently used by VRML models
 *      A typical operation on a VRML file would be something like this:
 *      1. Choose an output filename, a material appearance, and a
 *          VRML Transform name
 *      2. Invoke SetupVRML to write the VRML header
 *      3. Invoke SetupXForm to create a VRML Transform block
 *      4. Invoke SetupShape to create a Shape as a child within
 *          the VRML Transform block and a Geometry block within the Shape
 *      6. Write the Coordinate List to the file
 *      7. Write the Coordinate Index to the file
 *      8. Invoke CloseShape to close the Shape and Geometry blocks
 *      9. Repeat steps 4..8 to add more shapes to the Transform block
 *      10. Invoke CloseXForm to close the transform
 *      11. Close the file stream
 */

#ifndef VCOM_H_
#define VCOM_H_

#include <iosfwd>

class VRMLMat;

// Create the VRML header and ancillary comments
int SetupVRML(std::string filename, std::ofstream &fp);

// Set up a VRML Transform block
int SetupXForm(std::string name, std::ofstream &fp, int tabs = 0);

// Close a VRML Transform block
int CloseXForm(std::ofstream &fp, int tabs = 0);

// Set up a VRML Shape block (includes appearance and geometry)
int SetupShape(VRMLMat &color, bool reuse_color,
        std::ofstream &fp, int tabs = 0);

// Close a VRML Shape block
int CloseShape(std::ofstream &fp, int tabs = 0);

// Set up a VRML Geometry block
int SetupGeom(std::ofstream &fp, int tabs = 0);

#endif /* VCOM_H_ */
