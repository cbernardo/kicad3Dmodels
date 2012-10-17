/*
 file: dip_pin.cpp

 Copyright 2012, Cirilo Bernardo

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

 TODO:
 * check constraints
 */

#include <fstream>
#include <iomanip>
#include <cmath>

#include "dip_pin.h"
#include "dip_limits.h"

using namespace std;

dippin::dippin()
{
    /* defaults are in inches for PDIP-24 */
    BEV = 0.002;
    A1 = 0.015;
    A2 = 0.13;
    B = 0.018; /* pin through width */
    B1 = 0.06; /* pin width */
    B2 = 0.015; /* length of tapered portion */
    C1 = 0.01; /* length of top bent portion) */
    C2 = 0.0025; /* offset of lower bent portion*/
    C = 0.01; /* pin thickness */
    E1 = 0.26; /* package width */
    E = 0.3; /* pin row spacing */
    L = 0.13; /* pin length (narrow part) */
    scale = 10.0;
    ismetric = false;
    valid = false;
    return;
}

void dippin::setMetric(bool metric)
{
    if (metric && ismetric)
        return;
    if ((!metric) && (!ismetric))
        return;

    /* scale variables */
    if (metric)
    {
        BEV *= 25.4;
        A1 *= 25.4;
        A2 *= 25.4;
        B *= 25.4;
        B1 *= 25.4;
        B2 *= 25.4;
        C1 *= 25.4;
        C2 *= 25.4;
        C *= 25.4;
        E1 *= 25.4;
        E *= 25.4;
        L *= 25.4;
        scale *= 25.4;
        ismetric = true;
        valid = false;
    }
    else
    {
        BEV /= 25.4;
        A1 /= 25.4;
        A2 /= 25.4;
        B /= 25.4;
        B1 /= 25.4;
        B2 /= 25.4;
        C1 /= 25.4;
        C2 /= 25.4;
        C /= 25.4;
        E1 /= 25.4;
        E /= 25.4;
        L /= 25.4;
        scale /= 25.4;
        ismetric = false;
        valid = false;
    }
}

int dippin::setBaseHeight(double a1)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((a1 * ls) < MIN_BASE_HEIGHT)
    ||((a1*ls) > MAX_BASE_HEIGHT))return -1;
    A1 = a1;
    return 0;
}

int dippin::setCaseDepth(double a2)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((a2 * ls) < MIN_CASE_DEPTH)
    ||((a2*ls) > MAX_CASE_DEPTH))return -1;
    A2 = a2;
    return 0;
}

int dippin::setThruWidth(double b)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((b * ls) < MIN_PINWIDTH)
    ||((b*ls) > MAX_PINWIDTH))return -1;
    B = b;
    return 0;
}

int dippin::setPinBreadth(double b1)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((b1 * ls) < MIN_PINBREADTH)
    ||((b1*ls) > MAX_PINBREADTH))return -1;
    B1 = b1;
    return 0;
}

int dippin::setTaperHeight(double b2)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((b2 * ls) < MIN_TAPERHEIGHT)
    ||((b2*ls) > MAX_TAPERHEIGHT))return -1;
    B2 = b2;
    return 0;
}

int dippin::setDogEar(double c1)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((c1 * ls) < MIN_DOGEAR)
    ||((c1*ls) > MAX_DOGEAR))return -1;
    C1 = c1;
    return 0;
}

int dippin::setDogEarOffset(double c2)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((c2 * ls) < MIN_DOGOFF)
    ||((c2*ls) > MAX_DOGOFF))return -1;
    C2 = c2;
    return 0;
}

int dippin::setThickness(double c)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((c * ls) < MIN_THICK)
    ||((c*ls) > MAX_THICK))return -1;
    C = c;
    return 0;
}

int dippin::setCaseWidth(double e1)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((e1 * ls) < MIN_CASEWIDTH)
    ||((e1*ls) > MAX_CASEWIDTH))return -1;
    E1 = e1;
    return 0;
}

int dippin::setRowSpacing(double e)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((e * ls) < MIN_ROWSPACE)
    ||((e*ls) > MAX_ROWSPACE))return -1;
    E = e;
    return 0;
}

int dippin::setThruLength(double l)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((l * ls) < MIN_THRU)
    ||((l*ls) > MAX_THRU))return -1;
    L = l;
    return 0;
}

Quat *dippin::calc(void)
{
    int i;
    double height;

    /* TODO: check constraints */

    // points 0 .. 3 -- top, CCW from LH on case
    height = A1 + (A2 + C) / 2.0;
    p[0].z = p[1].z = p[2].z = p[3].z = height;
    p[0].x = p[1].x = -B1 / 2.0;
    p[2].x = p[3].x = -p[0].x;
    p[0].y = p[3].y = 0.0;
    p[1].y = p[2].y = (E1 - E - C) / 2.0 + C1;
    // points 4 .. 11 -- leg, front
    p[4].z = p[11].z = p[0].z - C1;
    p[5].z = p[10].z = B2;
    p[6].z = p[9].z = 0.0;
    p[7].z = p[8].z = -L + BEV;
    p[4].x = p[5].x = p[0].x;
    p[11].x = p[10].x = p[3].x;
    p[6].x = p[7].x = -B / 2.0;
    p[8].x = p[9].x = -p[6].x;
    p[4].y = p[5].y = p[6].y = p[7].y = p[1].y - C1;
    p[8].y = p[9].y = p[10].y = p[11].y = p[4].y;
    // points 12 .. 15 -- top, rear face CCW from LH on case
    p[12].z = p[13].z = p[14].z = p[15].z = p[0].z - C;
    p[12].x = p[13].x = p[0].x;
    p[14].x = p[15].x = p[3].x;
    p[12].y = p[15].y = 0.0;
    p[13].y = p[14].y = p[1].y - C1 + C + C2;
    // points 16 .. 23 -- leg, rear
    p[16].z = p[23].z = p[12].z - C2;
    p[17].z = p[22].z = p[5].z;
    p[18].z = p[21].z = p[6].z;
    p[19].z = p[20].z = p[7].z;
    p[16].x = p[17].x = p[4].x;
    p[22].x = p[23].x = p[11].x;
    p[18].x = p[19].x = p[6].x;
    p[20].x = p[21].x = p[8].x;
    p[16].y = p[17].y = p[18].y = p[19].y = p[4].y + C;
    p[20].y = p[21].y = p[22].y = p[23].y = p[16].y;
    // points 24 .. 35 -- left face
    p[24].z = p[25].z = p[12].z + BEV;
    p[26].z = p[16].z + BEV;
    p[27].z = p[32].z = p[17].z;
    p[28].z = p[31].z = p[18].z;
    p[29].z = p[30].z = p[19].z;
    p[33].z = p[11].z - BEV;
    p[34].z = p[35].z = p[0].z - BEV;
    p[24].x = p[25].x = p[26].x = p[27].x = p[0].x - BEV;
    p[32].x = p[33].x = p[34].x = p[35].x = p[24].x;
    p[28].x = p[29].x = p[30].x = p[31].x = p[6].x - BEV;
    p[24].y = p[35].y = p[0].y;
    //p[25].y = p[34].y = p[13].y - BEV;
    p[25].y = p[13].y - BEV;
    p[34].y = p[2].y + BEV;
    p[26].y = p[27].y = p[28].y = p[29].y = p[16].y - BEV;
    p[30].y = p[31].y = p[32].y = p[33].y = p[4].y + BEV;
    // points 36 .. 47 -- right face
    p[36].z = p[37].z = p[2].z - BEV;
    p[38].z = p[11].z - BEV;
    p[46].z = p[47].z = p[14].z + BEV;
    p[39].z = p[44].z = p[10].z;
    p[40].z = p[43].z = p[9].z;
    p[41].z = p[42].z = p[8].z;
    p[45].z = p[23].z;
    p[36].x = p[37].x = p[38].x = p[39].x = p[3].x + BEV;
    p[44].x = p[45].x = p[46].x = p[47].x = p[36].x;
    p[40].x = p[41].x = p[9].x + BEV;
    p[42].x = p[43].x = p[21].x + BEV;
    p[36].y = p[47].y = p[3].y;
    //p[37].y = p[46].y = p[2].y + BEV;
    p[37].y = p[2].y + BEV;
    p[46].y = p[22].y + BEV;
    p[38].y = p[39].y = p[40].y = p[41].y = p[11].y + BEV;
    p[42].y = p[43].y = p[44].y = p[45].y = p[23].y - BEV;
    // points 48 .. 51 -- bottom
    p[48].z = p[49].z = p[50].z = p[51].z = -L;
    p[48].x = p[51].x = B / 2.0 - BEV;
    p[49].x = p[50].x = -p[48].x;
    p[48].y = p[49].y = p[4].y + BEV;
    p[50].y = p[51].y = p[16].y - BEV;
    for (i = 0; i < PIN_NP; ++i)
        p[i] = p[i] * scale;
    valid = true;
    return p;
}

int dippin::writeCoord(std::ofstream &fp, int tabs, Transform *t)
{
    int i;
    Quat loc;

    if (!valid)
        return -1;

    if (tabs < 0) tabs = 0;
    if (tabs > 4) tabs = 4;
    string fmt(4*tabs, ' ');

    fp << fmt << " coord Coordinate { point [\n";
    fp << fmt << "   ";
    for (i = 0; i < PIN_NP - 1; ++i)
    {
        if (t)
        {
            loc = p[i];
            t->transform(loc);
            fp << setprecision(3) << " " << loc.x << " " << loc.y << " " << loc.z << ",";
        }
        else
        {
            fp << setprecision(3) << " " << p[i].x << " " << p[i].y << " " << p[i].z << ",";
        }
        if (!((i + 1) % 6))
            fp << "\n" << fmt << "   ";
    }
    if (t)
    {
        loc = p[i];
        t->transform(loc);
        fp << setprecision(3) << " " << loc.x << " " << loc.y << " " << loc.z << " ]\n";
        fp << fmt << "}\n";
    }
    else
    {
        fp << setprecision(3) << " " << p[i].x << " " << p[i].y << " " << p[i].z << " ]\n";
        fp << fmt << "}\n";
    }

    return fp.good() ? 0 : -1;
}

/* write out the coordinate indices for the facets */
int dippin::writeFacets(std::ofstream &fp, int tabs)
{
    if (tabs < 0) tabs = 0;
    if (tabs > 4) tabs = 4;
    string fmt(4*tabs, ' ');

    fp << fmt << "coordIndex [\n";
    /* pin front, top and angle */
    fp << fmt << "    0, 1, 2, 3, -1, 1, 4, 11, 2, -1,\n";
    /* pin front, chevron */
    fp << fmt << "    4, 5, 6, 9, 10, 11, -1,\n";
    /* pin front, pin */
    fp << fmt << "    6, 7, 8, 9, -1,\n";
    /* pin rear, top and angle */
    fp << fmt << "    15, 14, 13, 12, -1, 14, 23, 16, 13, -1,\n";
    /* pin rear, chevron */
    fp << fmt << "    23, 22, 21, 18, 17, 16, -1,\n";
    /* pin rear, pin */
    fp << fmt << "    21, 20, 19, 18, -1,\n";
    /* pin left side */
    fp << fmt << "    24, 25, 34, 35, -1, 25, 26, 33, 34, -1,\n";
    fp << fmt << "    26, 27, 32, 33, -1,  27, 28, 31, 32, -1,\n";
    fp << fmt << "    28, 29, 30, 31, -1,\n";
    /* right side */
    fp << fmt << "    36, 37, 46, 47, -1, 37, 38, 45, 46, -1,\n";
    fp << fmt << "    38, 39, 44, 45, -1, 39, 40, 43, 44, -1,\n";
    fp << fmt << "    40, 41, 42, 43, -1,\n";
    /* pin bottom */
    fp << fmt << "    48, 49, 50, 51, -1,\n";
    /* pin bevel, bottom */
    fp << fmt << "    49, 48, 8, 7, -1, 51, 50, 19, 20, -1,\n";
    fp << fmt << "    50, 49, 30, 29, -1, 48, 51, 42, 41, -1,\n";
    fp << fmt << "    30, 49, 7, -1, 29, 19, 50, -1,\n";
    fp << fmt << "    20, 42, 51, -1, 41, 8, 48, -1,\n";
    /* pin bevel, left */
    fp << fmt << "    35, 34, 1, 0, -1, 34, 33, 4, 1, -1,\n";
    fp << fmt << "    33, 32, 5, 4, -1, 32, 31, 6, 5, -1,\n";
    fp << fmt << "    31, 30, 7, 6, -1, 29, 28, 18, 19, -1,\n";
    fp << fmt << "    28, 27, 17, 18, -1, 27, 26, 16, 17, -1,\n";
    fp << fmt << "    26, 25 13, 16, -1, 25, 24, 12, 13, -1,\n";
    /* pin bevel, right */
    fp << fmt << "    3, 2, 37, 36, -1, 2, 11, 38, 37, -1,\n";
    fp << fmt << "    11, 10, 39, 38, -1, 10, 9, 40, 39, -1,\n";
    fp << fmt << "    9, 8, 41, 40, -1, 43, 42, 20, 21, -1,\n";
    fp << fmt << "    44, 43, 21, 22, -1, 45, 44, 22, 23, -1,\n";
    fp << fmt << "    46, 45, 23, 14, -1, 47, 46, 14, 15, -1,\n";
    /* close the surface; this facet is obscured by the case */
    fp << fmt << "    0, 3, 36, 47, 15, 12, 24, 35, -1\n" << fmt << "]\n";

    return fp.good() ? 0 : -1;
}
