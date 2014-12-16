/*
 *  file: dip_pin.h
 *
 *  Copyright 2012-2014 Cirilo Bernardo (cjh.bernardo@gmail.com)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 *  Notes:
 *  These classes were created to support the
 *  programmatic creation of VRML models for
 *  use with KiCad.
 *
 */

#ifndef DIP_PIN_H
#define DIP_PIN_H

#include <iosfwd>

#include <pin.h>
#include <transform.h>
#include <vrmlmat.h>


#define PIN_NP (52)

namespace KC3DDIP
{
class DIPPIN
{
private:
    KC3D::PIN pin[2];
    bool valid;

public:
    DIPPIN();

    /**
     * Calculate the vertices
     *
     * @param aPinHeight [in] height from the top of PCB to attachment point on case
     * @param aPinDepth [in] height from top of PCB to bottom of pin
     * @param aPinHLength [in] distance from pin axis to case
     * @param aPinThickness [in] material thickness (typically 0.014 in)
     * @param aPinBreadth [in] pin breadth (nominally the B2 specification)
     * @param aThruBreadth [in] thru breadth (nominally the B specification)
     * @param aPinBevel [in] bevel; typically aPinThickness/10
     * @return 0 for success, -1 for failure
     */
    int Calc( double aPinHeight, double aPinDepth, double aPinHLength,
              double aPinThickness, double aPinBreadth, double aThruBreadth,
              double aPinBevel );

    /**
     * Write the shape information to a file
     *
     * @param aTransform [in] transform to apply to results
     * @param aPinMaterial [in] material appearance
     * @param reuseMaterial [in] false to write out complete material appearance data
     * @param aVRMLFile [in] open output file
     * @param aTabDepth [in] formatting indent level
     * @return 0 for success, -1 for failure
     */
    int Build( KC3D::TRANSFORM& aTransform, KC3D::VRMLMAT& aPinMaterial,
            bool reuseMaterial, std::ofstream& aVRMLFile, int aTabDepth = 0 );
};

}    // namespace KC3DDIP

#endif // DIP_PIN_H
