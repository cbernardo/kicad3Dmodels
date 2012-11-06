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

/**
 * \brief Create the VRML header and ancillary comments.
 *
 * This procedure opens the specified file and writes out
 * a boilerplate VRML2.0 header which includes a permissive
 * license for the use and distribution of the output file.
 *
 * @param filename [in] name of file to create or open
 * @param fp [in,out]
 * @return 0 for success, -1 for failure
 */
int SetupVRML(std::string filename, std::ofstream &fp);

/**
 * \brief Set up a VRML Transform block
 *
 * This procedure writes the opening statements of
 * a VRML2.0 Transform block. In its most basic
 * usage it is called after SetupVRML then information
 * is added via calls to SetupShape, WriteCoord,
 * SetupCoordIndex, CloseCoordIndex, CloseShape,
 * and finally CloseXForm is invoked to close the
 * transform block.
 *
 * @param name [in] a VRML2.0 compliant name for the Transform block
 * @param fp   [in] open output file
 * @param tabs [in] indent level for formatting
 * @return 0 for success, -1 for failure
 */
int SetupXForm(std::string name, std::ofstream &fp, int tabs = 0);

/**
 * \brief Close a VRML Transform block
 *
 * Write out the closing braces to match a previous
 * call to SetupXForm.
 *
 * @param fp    [in] open output file
 * @param tabs  [in] indent level for formatting
 * @return 0 for success, -1 for failure
 */
int CloseXForm(std::ofstream &fp, int tabs = 0);

/**
 * \brief Set up a VRML Shape block
 *
 * Write the opening statements of a VRML2.0 Shape block. The
 * statements include Appearance and Geometry opening statements.
 *
 * @param color         [in] VRMLMat material appearance specification
 * @param reuse_color   [in] If FALSE the entire material specification is written, otherwise a previous definition is reused
 * @param fp            [in] open output file
 * @param tabs          [in] indent level for formatting
 * @return 0 for success, -1 for failure
 */
int SetupShape(VRMLMat &color, bool reuse_color,
        std::ofstream &fp, int tabs = 0);

/**
 * \brief Close a VRML Shape block
 *
 * Write the closing braces for a VRML2.0 Shape block previously
 * opened via a call to SetupShape.
 *
 * @param fp    [in] open output file
 * @param tabs  [in] indent level for formatting
 * @return
 */
int CloseShape(std::ofstream &fp, int tabs = 0);

/**
 * \brief Write a VRML coordinate block
 *
 * This procedure writes out a VRML2.0 Coordinate block
 * to specify vertices for use within a Geometry block.
 *
 * @param x     [in] pointer to array of vertex X coordinates
 * @param y     [in] pointer to array of vertex Y coordinates
 * @param z     [in] pointer to array of vertex Z coordinates
 * @param np    [in] number of points to write
 * @param fp    [in] open output file
 * @param tabs  [in] indent level for formatting
 * @return  0 for success, -1 for failure
 */
int WriteCoord(double *x, double *y, double *z, int np, std::ofstream &fp, int tabs = 0);

/**
 * \brief Set up a VRML coordIndex block
 *
 * This procedure writes the opening statement to a VRML2.0
 * coordinateIndex block for use within a Geometry block to
 * specify the vertices which make up each surface facet of
 * an object. The indices themselves must be written out by
 * the user before closing the block via CloseCoordIndex.
 * See the VRML2.0 specification for information on the
 * formatting of the vertex list.
 *
 * @param fp    [in] open output file
 * @param tabs  [in] indent level for formatting
 * @return 0 for success, -1 for failure
 */
int SetupCoordIndex(std::ofstream &fp, int tabs = 0);

/**
 * \brief Close a VRML coordIndex block
 *
 * Close a VRML2.0 coordIndex block which was previously
 * opened via a call to SetupCoordIndex.
 *
 * @param fp    [in] open output file
 * @param tabs  [in] indent level for formatting
 * @return
 */
int CloseCoordIndex(std::ofstream &fp, int tabs = 0);

#endif /* VCOM_H_ */
