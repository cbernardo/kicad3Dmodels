/*
 *      file: colorcodes.h
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

#ifndef COLORCODES_H
#define COLORCODES_H

#include <iosfwd>

namespace KC3DRESISTOR
{
// Return an index into the color table for the given tolerance code.
// If the code is invalid -1 is returned.
// Tolerances codes are as follows:
// Character Code  :   Index   : Tolerance :   Color
// none, or "-"    :   12      : 20%       :   same as resistor body
// K               :   11      : 10%       :   silver
// J               :   10      : 5%        :   gold
// G               :   2       : 2%        :   red
// F               :   1       : 1%        :   brown
// D               :   5       : 0.5%      :   green
// C               :   6       : 0.25%     :   blue
// B               :   7       : 0.1%      :   violet
// A               :   8       : 0.05%     :   gray
int GetToleranceIndex( const char c );

// return the number of bands needed to represent the given tolerance
// if the tolerance code is invalid and not 0 (default 20%), -1 is returned
int GetNBands( const char c );

// Return an index into the color table for the given Temperature Coefficient
// code. If the code is invalid, -1 is returned.
// Coefficient codes are as follows:
// Character Code  :   Index   : ppm/K :   Color
// U               :   0       : 250   :   black
// S               :   1       : 100   :   brown
// R               :   2       : 50    :   red
// P               :   3       : 15    :   orange
// Q               :   4       : 25    :   yellow
// M               :   7       : 5     :   violet
// K               :   8       : 1     :   gray
//
// 5               :   5       : 20    :   green
// 6               :   6       : 10    :   blue
//
// The 20% and 10% cases are special; the IEC code is 'Z', which means the
// temperature coefficient is specified in other documentation.  In reality
// manufacturers have been using green and blue as the actual colors. For
// the purposes of building models we avoid ambiguity by using 5, 6 rather
// than the standard 'Z' specifier.
int GetTCIndex( const char c );

// Print out the resistor tolerance codes
void printTolCodes( void );

// Print out the Tenperature Coefficient codes
void printTCCodes( void );

// Create the standard value designator for a 5-band code.
// The value must be in the range 100 .. 999 and the
// exponent may be -2 .. 10. Fractional value resistances
// are a special case and are not covered here.
int GetValueCode5( int value, int exp, std::string& vcode );

// Create the standard value designator for a 4-band code.
// The value must be in the range 100 .. 990 and the
// exponent may be -2 .. 8. Fractional value resistances
// are a special case not represented here.
int GetValueCode4( int value, int exp, std::string& vcode );
}    // namespace KC3DRESISTOR

#endif // COLORCODES_H
