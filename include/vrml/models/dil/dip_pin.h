/*
 file: dip_pin.h

 Copyright 2012, Cirilo Bernardo (cjh.bernardo@gmail.com)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>

 Notes:
 These classes were created to support the
 programmatic creation of VRML models for
 use with KiCad.

 */

#ifndef DIP_PIN_H
#define DIP_PIN_H

#include <iosfwd>

#include "pin.h"
#include "transform.h"
#include "vrmlmat.h"


#define PIN_NP (52)

namespace kc3ddip {

class DipPin
{
private:
    kc3d::Pin pin[2];
    bool valid;

public:
    DipPin();

    /**
     * Calculate the vertices
     *
     * @param h [in] height from the top of PCB to attachment point on case
     * @param l [in] height from top of PCB to bottom of pin
     * @param w [in] distance from pin axis to case
     * @param t [in] material thickness (typically 0.014 in)
     * @param pb [in] pin breadth (nominally the B2 specification)
     * @param tb [in] thru breadth (nominally the B specification)
     * @param bev [in] bevel; typically t/10
     * @return 0 for success, -1 for failure
     */
    int Calc(double h, double l, double w, double t, double pb, double tb, double bev);

    /**
     * Write the shape information to a file
     *
     * @param t [in] transform to apply to results
     * @param color [in] material appearance
     * @param reuse [in] false to write out complete material appearance data
     * @param file [in] open output file
     * @param tabs [in] formatting indent level
     * @return 0 for success, -1 for failure
     */
    int Build(kc3d::Transform &t, kc3d::VRMLMat &color, bool reuse, std::ofstream &file, int tabs = 0);
};

}   // namespace kc3ddip

#endif /* DIP_PIN_H */
