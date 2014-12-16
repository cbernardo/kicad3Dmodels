/*
 *  file: dip.h
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
 *   These classes were created to support the
 *   programmatic creation of VRML models for
 *   use with KiCad.
 *
 *   This class represents a parameterized DIP 3D model.
 *
 */

#ifndef DIP_H
#define DIP_H

#include <iosfwd>
#include <string>

#include <vrmlmat.h>

namespace KC3DDIP
{

struct DIPPARAMS
{
    // typical engineering information provided:
    double  A1; // height from top of PCB to bottom of case
    double  A2; // height of case
    double  L;  // depth of pin from top of PCB
    double  e;  // pin pitch
    double  E;  // row spacing
    double  E1; // case width
    double  B1; // pin breadth
    double  b;  // breadth of thru portion of pin
    double  c;  // pin material thickness, typical max 0.014 in

    // extra information:
    double  NW;         // notch width
    double  ND;         // notch depth
    double  NL;         // notch length
    double  casebev;    // bevel on case (nominal 0.005 in)
    double  pinbev;     // bevel on pin, nominally 1/10*c

    // other parameters (calculated if set < 0)
    double  MID;    // width of middle case portion where pins attach (nominal 1.5*C)
    double  DW;     // length allowance on case (nominal 2*(c/3 + 1/2*B1)
    double  S;      // case taper; deviation of unbeveled top and bottom edges from
                    // mid section. Typical specs are for a 4..9 deg incline; we use 5
    double scale;   // KiCAD world scale factor

    DIPPARAMS();
};

class DIP
{
private:
    DIPPARAMS params;
    int pins;
    bool* haspin;
    KC3D::VRMLMAT   pinmaterial;
    KC3D::VRMLMAT   casematerial;

public:
    DIP();
    virtual ~DIP();

    // set the DIP parameters except for pin number
    int SetParams( const DIPPARAMS& aParams );

    // set the nominal number of pins
    int SetPins( int aNPins );

    // control whether a pin is rendered (on) or not
    int SetPin( int aPinNumber, bool isRendered );

    // create the model
    int Build( std::string aVRMLFilename );

    // Set the pin color
    int SetPinColor( std::string aPinMatFName );

    // Set the case color
    int SetCaseColor( std::string aCaseMatFName );
};

}    // namespace kc3ddip

#endif
