/*
 *      file: makeE192.cpp
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



#include <unistd.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cctype>

#include "vdefs.h"
#include "colorcodes.h"

extern char *optarg;
extern int optopt;

using namespace std;
using namespace kc3dresistor;

// minimum and maximum decades
#define MIN_DECADE (0)
#define MAX_DECADE (10)

// minimum and maximum gaps in color code
#define MIN_GAP (0)
#define MAX_GAP (4)

void printUsage(void)
{
    cout << "\n\nThis software generates an Ennn series of filenames and color codes\n";
    cout << "based on the following command line parameters:\n\n";
    cout << "\t-e : E series (192, 96, 48, 24, 12) [Unimplemented: E12, E6, E3], default 24\n";
    cout << "\t-l : Minimum Decade (0 .. 10, default 0)\n";
    cout << "\t-u : Maximum Decade (0 .. 10, 0 = 1R00..9R88, ..., default 6)\n";
    cout << "\t-p : Part Series (for example, LR0204)\n";
    cout << "\t-t : tolerance code (optional, default to F)\n";
    cout << "\t-c : Temperature coefficient code (optional, default = no code)\n";
    cout << "\t-g : width of gap between bands, units = width of color band/2 (optional, default 1)\n\n";
    cout << "\t-x : Max base number in highest decade (100 .. 999) (optional)\n";
    printTolCodes();
    cout << "\n";
    printTCCodes();
    cout << "\n";
    return;
}


// Resistor series E192 (1%) according to IEC 60063
// ================================================

const int e192[192] = { 100, 101, 102, 104, 105, 106, 107, 109, 110, 111, 113, 114,
        115, 117, 118, 120, 121, 123, 124, 126, 127, 129, 130, 132,
        133, 135, 137, 138, 140, 142, 143, 145, 147, 149, 150, 152,
        154, 156, 158, 160, 162, 164, 165, 167, 169, 172, 174, 176,
        178, 180, 182, 184, 187, 189, 191, 193, 196, 198, 200, 203,
        205, 208, 210, 213, 215, 218, 221, 223, 226, 229, 232, 234,
        237, 240, 243, 246, 249, 252, 255, 258, 261, 264, 267, 271,
        274, 277, 280, 284, 287, 291, 294, 298, 301, 305, 309, 312,
        316, 320, 324, 328, 332, 336, 340, 344, 348, 352, 357, 361,
        365, 370, 374, 379, 383, 388, 392, 397, 402, 407, 412, 417,
        422, 427, 432, 437, 442, 448, 453, 459, 464, 470, 475, 481,
        487, 493, 499, 505, 511, 517, 523, 530, 536, 542, 549, 556,
        562, 569, 576, 583, 590, 597, 604, 612, 619, 626, 634, 642,
        649, 657, 665, 673, 681, 690, 698, 706, 715, 723, 732, 741,
        750, 759, 768, 777, 787, 796, 806, 816, 825, 835, 845, 856,
        866, 876, 887, 898, 909, 920, 931, 942, 953, 965, 976, 988 };

const int e96[96] = { 100, 102, 105, 107, 110, 113, 115, 118, 121, 124, 127, 130,
        133, 137, 140, 143, 147, 150, 154, 158, 162, 165, 169, 174,
        178, 182, 187, 191, 196, 200, 205, 210, 215, 221, 226, 232,
        237, 243, 249, 255, 261, 267, 274, 280, 287, 294, 301, 309,
        316, 324, 332, 340, 348, 357, 365, 374, 383, 392, 402, 412,
        422, 432, 442, 453, 464, 475, 487, 499, 511, 523, 536, 549,
        562, 576, 590, 604, 619, 634, 649, 665, 681, 698, 715, 732,
        750, 768, 787, 806, 825, 845, 866, 887, 909, 931, 953, 976 };

const int e48[48] = { 100, 105, 110, 115, 121, 127, 133, 140, 147, 154, 162, 169,
        178, 187, 196, 205, 215, 226, 237, 249, 261, 274, 287, 301,
        316, 332, 348, 365, 383, 402, 422, 442, 464, 487, 511, 536,
        562, 590, 619, 649, 681, 715, 750, 787, 825, 866, 909, 953 };

const int e24[24] = { 100, 110, 120, 130, 150, 160, 180, 200, 220, 240, 270, 300,
        330, 360, 390, 430, 470, 510, 560, 620, 680, 750, 820, 910 };

const int e12[12] = { 100, 120, 150, 180, 220, 270, 330, 390, 470, 560, 680, 820 };


int main(int argc, char **argv)
{

    int ldec = 0;       // minimum decade
    int udec = 6;       // maximum decade
    string series;      // Part Series (for example LR0204)
    char tc = 0;        // temperature coefficient code
    char tol = 'F';     // tolerance code (default 'F', 1%)
    string gap = "X";   // gap between color bands
    int nser = 96;      // series number (default E96)
    const int *sp;      // pointer to the series
    int nb = 5;         // number of bands (default 5), depends on 'tol'
    int tmax = 999;     // max value at top decade

    int i;
    int ic;
    istringstream ts;

    while ((ic = getopt(argc, argv, ":l:u:p:t:c:g:x:e:h")) > 0)
    {
        if (ic == ':')
        {
            ERRBLURB;
            cerr << "missing option to '-" << (char)optopt << "'\n";
            printUsage();
            return -1;
        }
        if (ic == '?')
        {
            ERRBLURB;
            cerr << "unrecognized option '-" << (char)optopt << "'\n";
            printUsage();
            return -1;
        }
        if (ic == 'h')
        {
            printUsage();
            return 0;
        }
        if (ic == 'l')
        {
            // minimum decade
            ts.clear();
            ts.str(optarg);
            i = -10;
            ts >> i;
            if ((i < MIN_DECADE)||(i > MAX_DECADE))
            {
                ERRBLURB;
                cerr << "invalid minimum decade (" << i << "); accepted values are " <<
                        MIN_DECADE << " to " << MAX_DECADE << "\n";
                cerr << "code from command line: '" << ts.str().c_str() << "'\n";
                printUsage();
                return -1;
            }
            ldec = i;
            continue;
        }
        if (ic == 'u')
        {
            // maximum decade
            ts.clear();
            ts.str(optarg);
            i = -10;
            ts >> i;
            if ((i < MIN_DECADE)||(i > MAX_DECADE))
            {
                ERRBLURB;
                cerr << "invalid maximum decade (" << i << "); accepted values are " <<
                        MIN_DECADE << " to " << MAX_DECADE << "\n";
                cerr << "code from command line: '" << ts.str().c_str() << "'\n";
                printUsage();
                return -1;
            }
            udec = i;
            continue;
        }
        if (ic == 'e')
        {
            // Preferred values series (192, 96, 48, 24, 12)
            ts.clear();
            ts.str(optarg);
            i = -10;
            ts >> i;
            if ((i != 192) && (i != 96) && (i != 48)
                    && (i != 24) && (i != 12))
            {
                ERRBLURB;
                cerr << "invalid preferred values code (" << i << "); " <<
                        "accepted values are 192, 96, 48, 24, and 12\n";
                cerr << "code from command line: '" << ts.str().c_str() << "'\n";
                printUsage();
                return -1;
            }
            nser = i;
            continue;
        }
        if (ic == 'x')
        {
            // Preferred values series (192, 96, 48, 24, 12)
            ts.clear();
            ts.str(optarg);
            i = -10;
            ts >> i;
            if ((i < 100) || (i > 999))
            {
                ERRBLURB;
                cerr << "invalid value limit (" << i << "); " << "accepted values are 100 .. 999\n";
                cerr << "code from command line: '" << ts.str().c_str() << "'\n";
                printUsage();
                return -1;
            }
            tmax = i;
            continue;
        }
        if (ic == 'p')
        {
            // part series
            series = optarg;
            continue;
        }
        if (ic == 't')
        {
            // value tolerance
            tol = *optarg;
            continue;
        }
        if (ic == 'c')
        {
            // temperature coefficient
            tc = *optarg;
            continue;
        }
        if (ic == 'g')
        {
            // gap width
            ts.clear();
            ts.str(optarg);
            i = -10;
            ts >> i;
            if ((i < MIN_GAP)||(i > MAX_GAP))
            {
                ERRBLURB;
                cerr << "invalid gap value; accepted values are " <<
                        MIN_GAP << " to " << MAX_GAP << "\n";
                printUsage();
                return -1;
            }
            gap.clear();
            gap.assign(i, 'X');
            continue;
        }
    }

    // validate the decades
    if (ldec > udec)
    {
        ERRBLURB;
        cerr << "lower decade (" << ldec << ") exceeds upper decade (" << udec << ")\n";
        return -1;
    }

    // make sure we have a valid tolerance code
    if ((tol) && (GetToleranceIndex(tol) < 0))
    {
        ERRBLURB;
        cerr << "invalid tolerance code ('" << tol << "'); code is case sensitive.\n";
        cerr << "Allowed values are one of:\n\n";
        printTolCodes();
        cerr << "\n";
        return -1;
    }
    // number of bands depends on tolerance
    nb = GetNBands(tol);

    // select the appropriate list
    switch (nser)
    {
    case 192:
        sp = e192;
        break;
    case 96:
        sp = e96;
        break;
    case 48:
        sp = e48;
        break;
    case 24:
        sp = e24;
        break;
    case 12:
        sp = e12;
        break;
    default:
        ERRBLURB;
        cerr << "unsupported E series (" << nser << ")\n";
        return -1;
        break;
    }

    // make sure we have a valid temperature coefficient
    if ((tc) && (GetTCIndex(tc) < 0))
    {
        ERRBLURB;
        cerr << "invalid temperature coefficient code ('" << tc << "'); code is case sensitive.\n";
        cerr << "Allowed values are one of:\n\n";
        printTCCodes();
        cerr << "\n";
        return -1;
    }

    // was a series specified?
    if (series.empty())
    {
        ERRBLURB;
        cerr << "\nNo series name specified. The name must begin with an alphabet character and the\n";
        cerr << "name may contain only alphanumeric characters, dash ('-'), and underscore ('_')\n";
        return -1;
    }

    // ensure that the series name begins with an Alpha character; no other
    // restrictions are placed although we should probably limit all succeeding
    // characters to one of Alpha, Digit, "-", and "_".
    if (!isalpha(series.c_str()[0]))
    {
        ERRBLURB;
        cerr << "\nInvalid series name. The name must begin with an alphabet character and the name \n";
        cerr << "may contain only alphanumeric characters, dash ('-'), and underscore ('_')\n";
        return -1;
    }

    // generate the control file
    cout << "# E" << nser << " series generator for Part Series " << series << "\n";
    cout << "# Decades " << ldec << " to " << udec << "\n";
    cout << "# Tolerance code : " << tol << "\n";
    cout << "# Temperature coefficient code : ";
    if ((!tc) || (tc == '-'))
        cout << "(none)\n";
    else
        cout << tc << "\n";
    cout << "# Gap setting: " << gap.length() << "\n";
    cout << "# Top of series <= " << tmax << "\n";
    cout << "# Number of bands = " << nb << " (automatic; file a bug report if it is incorrect)\n";

    // basename = [Part Series]_[Value]_[Tolerance][TCoeff]
    ostringstream basename;
    string codestr;

    int v0, v1, v2; // elements of the resistor code
    int j, k;
    char oc;        // temporary output character

    for (i = ldec; i <= udec; ++i)
    {
        cout << "\n# Decade " << i << "\n";
        for (j = 0; j < nser; ++j)
        {
            k = sp[j];
            v0 = k / 100;
            v1 = (k % 100)/10;
            v2 = k % 10;
            basename.str("");
            codestr.clear();
            switch (nb)
            {
            case 5:
                if (GetValueCode5(sp[j], i-2, codestr))
                {
                    ERRBLURB;
                    cerr << "val(" << sp[j] << "), exp(" << i-2 << ")\n";
                    continue;
                }
                break;
            case 4:
                if (GetValueCode4(sp[j], i-2, codestr))
                {
                    ERRBLURB;
                    cerr << "val(" << sp[j] << "), exp(" << i-2 << ")\n";
                    continue;
                }
                break;
            default:
                ERRBLURB;
                cerr << "invalid number of bands (" << nb << "); valid values are 4 or 5 only\n";
                continue;
                break;
            }
            basename << series << "_" << codestr << "_" << tol;
            if (tc)
                basename << tc;
            else
                basename << "-";
            k = i - 2;
            if (nb == 4) k += 1;
            if (k > 9)
            {
                ERRBLURB;
                cerr << "invalid exponent (>9); a decade > 11 must have been specified\n";
                continue;
            }
            if (k < -2)
            {
                ERRBLURB;
                cerr << "invalid exponent (<-2); a decade < 0 must have been specified\n";
                continue;
            }
            if (k >= 0)
            {
                oc = 48 + k;
            }
            else
            {
                if (k == -2) oc = 'S';
                if (k == -1) oc = 'G';
            }
            if (k == -1) k = 10;
            if (k == -2) k = 11;
            // base name + code up to Multiplier
            switch (nb)
            {
            case 5:
                cout << basename.str().c_str() << " : " << v0 << v0 << gap << v1 << v1 <<
                        gap << v2 << v2 << gap << oc << oc << gap << "XX" << gap;
                break;
            case 4:
                cout << basename.str().c_str() << " : " << v0 << v0 << gap << v1 << v1 <<
                        gap << oc << oc << gap << "XX" << gap;
                break;
            }
            // add in the tolerance
            k = GetToleranceIndex(tol);
            if (k < 0) k = 12;
            if (k < 10)
            {
                oc = 48 + k;
                cout << oc << oc << oc;
            }
            else
            {
                if (k == 10) cout << "GGG";
                if (k == 11) cout << "SSS";
                if (k == 12) cout << "XXX";
            }
            // add in the temperature coefficient
            if ((tc) && ((k = GetTCIndex(tc)) >= 0))
            {
                cout << gap;
                if (k < 10)
                {
                    oc = 48 + k;
                    cout << oc << oc;
                }
                else
                {
                    if (k == 10) cout << "GG";
                    if (k == 11) cout << "SS";
                }
            }
            cout << "\n";
        }
    }

    return 0;
} // main
