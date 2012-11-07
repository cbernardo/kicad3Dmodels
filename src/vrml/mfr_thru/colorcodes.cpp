/*
 *      file: colorcodes.cpp
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

#include <iostream>
#include <sstream>
#include <string>

#include "vdefs.h"

using namespace std;

// max. and min. exponents
#define MIN_EXP (-2)
#define MAX_EXP (9)

namespace kc3dresistor {

// Return an index into the color table for the given tolerance code.
int GetToleranceIndex(const char c)
{
    int val;

    switch (c)
    {
    case 'A':
        val = 8;
        break;
    case 'B':
        val = 7;
        break;
    case 'C':
        val = 6;
        break;
    case 'D':
        val = 5;
        break;
    case 'F':
        val = 1;
        break;
    case 'G':
        val = 2;
        break;
    case 'J':
        val = 10;
        break;
    case 'K':
        val = 11;
        break;
    case '-':
        val = 12;
        break;
    default:
        val = -1;
        break;
    }

    return val;
}

// return the number of bands needed to represent the given tolerance
int GetNBands(const char c)
{
    int nb;

    switch (c)
    {
    case 0:
    case '-':
    case 'K':
    case 'J':
        nb = 4;
        break;
    case 'G':
    case 'F':
    case 'D':
    case 'C':
    case 'B':
    case 'A':
        nb = 5;
        break;
    default:
        nb = -1;
        break;
    }
    return nb;
}

// Return an index into the color table for the given Temperature Coefficient
int GetTCIndex(const char c)
{
    int val;

    switch (c)
    {
    case 'U':
        val = 0;
        break;
    case 'S':
        val = 1;
        break;
    case 'R':
        val = 2;
        break;
    case 'P':
        val = 3;
        break;
    case 'Q':
        val = 4;
        break;
    case '5':
        val = 5;
        break;
    case '6':
        val = 6;
        break;
    case 'M':
        val = 7;
        break;
    case 'K':
        val = 8;
        break;
    default:
        ERRBLURB;
        cerr << "invalid temperature coefficient code\n";
        val = -1;
        break;
    }

    return val;
}

// Print out the resistor tolerance codes
void printTolCodes(void)
{
    cout << "Tolerance Codes:\n";
    cout << "\t- : 20    %\n";
    cout << "\tK : 10    %\n";
    cout << "\tJ :  5    %\n";
    cout << "\tG :  2    %\n";
    cout << "\tF :  1    %\n";
    cout << "\tD :  0.5  %\n";
    cout << "\tC :  0.25 %\n";
    cout << "\tB :  0.1  %\n";
    cout << "\tA :  0.05 %\n";
    return;
}

// Print out the Tenperature Coefficient codes
void printTCCodes(void)
{
    cout << "Temperature Coefficient Codes:\n";
    cout << "\tU : 250 ppm/K\n";
    cout << "\tS : 100 ppm/K\n";
    cout << "\tR :  50 ppm/K\n";
    cout << "\tQ :  25 ppm/K\n";
    cout << "\t5 :  20 ppm/K\n";
    cout << "\tP :  15 ppm/K\n";
    cout << "\t6 :  10 ppm/K\n";
    cout << "\tM :   5 ppm/K\n";
    cout << "\tK :   1 ppm/K\n";
    cout << "\t* note: the codes '5' and '6' are not standard notation but are used here\n";
    cout << "\t        to indicate the color band used and to eliminate ambiguity.\n";
    return;
}

// Create the standard value designator for a 5-band code.
//  The value must be in the range 100 .. 999 and the
//  exponent may be -2 .. 8. Fractional value resistances
//  are a special case and are not covered here.
int GetValueCode5(int value, int exp, std::string &vcode)
{
    vcode.clear();
    if ((value < 100)||(value > 999))
    {
        ERRBLURB;
        cerr << "invalid value code (" << value << "); range is 100 .. 999\n";
        return -1;
    }

    if ((exp < MIN_EXP)||(exp > MAX_EXP))
    {
        ERRBLURB;
        cerr << "invalid exponent code (" << value << "); range is " <<
                MIN_EXP << " to " << MAX_EXP << "\n";
        return -1;
    }

    int pos;    // position of the magnitude code (after 1st, 2nd or 3rd char)
    char mag;   // magnitude code (R, K, M, G)

    int v0, v1, v2;
    v0 = value/100;
    v1 = (value % 100)/10;
    v2 = (value % 10);

    switch (exp)
    {
    case -2:
        pos = 1;
        mag = 'R';
        break;
    case -1:
        pos = 2;
        mag = 'R';
        break;
    case 0:
        pos = 3;
        mag = 'R';
        break;
    case 1:
        pos = 1;
        mag = 'K';
        break;
    case 2:
        pos = 2;
        mag = 'K';
        break;
    case 3:
        pos = 3;
        mag = 'K';
        break;
    case 4:
        pos = 1;
        mag = 'M';
        break;
    case 5:
        pos = 2;
        mag = 'M';
        break;
    case 6:
        pos = 3;
        mag = 'M';
        break;
    case 7:
        pos = 1;
        mag = 'G';
        break;
    case 8:
        pos = 2;
        mag = 'G';
        break;
    case 9:
        pos = 3;
        mag = 'G';
        break;
    default:
        ERRBLURB;
        cerr << "Currently unsupported exponent: " << exp << "\n";
        return -1;
        break;
    }

    ostringstream os;
    switch(pos)
    {
    case 1:
        os << v0 << mag << v1 << v2;
        break;
    case 2:
        os << v0 << v1 << mag << v2;
        break;
    case 3:
        os << v0 << v1 << v2 << mag;
        break;
    }
    vcode = os.str();
    return 0;
}

// Create the standard value designator for a 4-band code.
//  The value must be in the range 100 .. 990 and the
//  exponent may be -2 .. 8. Fractional value resistances
//  are a special case not represented here.
int GetValueCode4(int value, int exp, std::string &vcode)
{
    vcode.clear();
    if ((value < 100)||(value > 990))
    {
        ERRBLURB;
        cerr << "invalid value code (" << value << "); range is 10 .. 990\n";
        return -1;
    }

    if ((exp < MIN_EXP)||(exp > MAX_EXP))
    {
        ERRBLURB;
        cerr << "invalid exponent code (" << value << "); range is " <<
                MIN_EXP << " to " << MAX_EXP << "\n";
        return -1;
    }

    int pos;    // position of the magnitude code (after 1st, 2nd or 3rd char)
    char mag;   // magnitude code (R, K, M, G)

    int v0, v1;
    v0 = value/100;
    v1 = (value % 100)/10;

    switch (exp)
    {
    case -2:
        pos = 1;
        mag = 'R';
        break;
    case -1:
        pos = 2;
        mag = 'R';
        break;
    case 0:
        pos = 3;
        mag = 'R';
        break;
    case 1:
        pos = 1;
        mag = 'K';
        break;
    case 2:
        pos = 2;
        mag = 'K';
        break;
    case 3:
        pos = 3;
        mag = 'K';
        break;
    case 4:
        pos = 1;
        mag = 'M';
        break;
    case 5:
        pos = 2;
        mag = 'M';
        break;
    case 6:
        pos = 3;
        mag = 'M';
        break;
    case 7:
        pos = 1;
        mag = 'G';
        break;
    case 8:
        pos = 2;
        mag = 'G';
        break;
    case 9:
        pos = 3;
        mag = 'G';
        break;
    default:
        ERRBLURB;
        cerr << "Currently unsupported exponent: " << exp << "\n";
        return -1;
        break;
    }

    ostringstream os;
    switch(pos)
    {
    case 1:
        os << v0 << mag << v1;
        break;
    case 2:
        os << v0 << v1 << mag;
        break;
    case 3:
        os << v0 << v1 << "0" << mag;
        break;
    }
    vcode = os.str();
    return 0;
}

}   // namespace kc3dresistor
