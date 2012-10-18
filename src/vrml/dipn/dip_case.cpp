/*
 file: dip_case.cpp

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

#include <cmath>
#include <iomanip>
#include <fstream>

#include "dip_case.h"
#include "dip_limits.h"

using namespace std;

dipcase::dipcase()
{
    /* defaults are in inches for PDIP-24 */
    NW = 0.06;
    NL = 0.08;
    ND = 0.02;
    BEV = 0.005;
    MID = 0.02;
    D = 1.25;
    E1 = 0.26;
    S = 0.01;
    A1 = 0.015;
    A2 = 0.13;
    scale = 10.0;
    ismetric = false;
    valid = false;
    return;
}

void dipcase::setMetric(bool metric)
{
    if (metric && ismetric)
        return;
    if ((!metric) && (!ismetric))
        return;

    if (metric)
    {
        NW *= 25.4;
        NL *= 25.4;
        ND *= 25.4;
        BEV *= 25.4;
        MID *= 25.4;
        D *= 25.4;
        E1 *= 25.4;
        S *= 25.4;
        A1 *= 25.4;
        A2 *= 25.4;
        scale *= 25.4;
        ismetric = true;
        valid = false;
    }
    else
    {
        NW /= 25.4;
        NL /= 25.4;
        ND /= 25.4;
        BEV /= 25.4;
        MID /= 25.4;
        D /= 25.4;
        E1 /= 25.4;
        S /= 25.4;
        A1 /= 25.4;
        A2 /= 25.4;
        scale /= 25.4;
        ismetric = false;
        valid = false;
    }
}

int dipcase::setCaseLength(double d)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((d * ls) < MIN_CASELEN) || ((d * ls) > MAX_CASELEN))
        return -1;
    D = d;
    return 0;
}

int dipcase::setCaseWidth(double e1)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((e1 * ls) < MIN_CASEWIDTH) || ((e1 * ls) > MAX_CASEWIDTH))
        return -1;
    E1 = e1;
    return 0;
}

int dipcase::setCaseTaper(double s)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((s * ls) < MIN_CASETAPER) || ((s * ls) > MAX_CASETAPER))
        return -1;
    S = s;
    return 0;
}

int dipcase::setBaseHeight(double a1)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((a1 * ls) < MIN_BASE_HEIGHT) || ((a1 * ls) > MAX_BASE_HEIGHT))
        return -1;
    A1 = a1;
    return 0;
}

int dipcase::setCaseDepth(double a2)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((a2 * ls) < MIN_CASE_DEPTH) || ((a2 * ls) > MAX_CASE_DEPTH))
        return -1;
    A2 = a2;
    return 0;
}

int dipcase::setNotchWidth(double nw)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((nw * ls) < MIN_NOTCHWIDTH) || ((nw * ls) > MAX_NOTCHWIDTH))
        return -1;
    NW = nw;
    return 0;
}

int dipcase::setNotchDepth(double nd)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((nd * ls) < MIN_NOTCHDEPTH) || ((nd * ls) > MAX_NOTCHDEPTH))
        return -1;
    ND = nd;
    return 0;
}

int dipcase::setCaseBevel(double bev)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((bev * ls) < MIN_CASEBEV) || ((bev * ls) > MAX_CASEBEV))
        return -1;
    BEV = bev;
    return 0;
}

int dipcase::setCaseMidHeight(double mid)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((mid * ls) < MIN_CASEMID) || ((mid * ls) > MAX_CASEMID))
        return -1;
    MID = mid;
    return 0;
}

int dipcase::setNotchLength(double nl)
{
    valid = false;
    double ls = 1.0;  // local scale factor to test limits
    if (ismetric)
        ls = 1.0 / 24.0;
    if (((nl * ls) < MIN_NOTCHLEN) || ((nl * ls) > MAX_NOTCHLEN))
        return -1;
    NL = nl;
    return 0;
}

int dipcase::setWorldScale(double sc)
{
    scale = sc;
    valid = false;
}

Quat *dipcase::calc(void)
{
    int i;
    double height;
    double slope;
    double off;
    double ndia;
    /* TODO: check constraints */

    ndia = NW / 2.0;
    slope = S / ((A2 - MID) / 2 - BEV);
    /* L1, points 0..4 */
    for (i = 0; i < 4; ++i)
        p[i].z = A1;
    p[0].x = -D / 2 + S + BEV;
    p[0].y = -E1 / 2 + S + BEV;
    p[1].x = -p[0].x;
    p[1].y = p[0].y;
    p[2].x = p[1].x;
    p[2].y = -p[1].y;
    p[3].x = p[0].x;
    p[3].y = p[2].y;
    /* L2, points 4..11 */
    height = A1 + BEV;
    for (i = 4; i < 12; ++i)
        p[i].z = height;
    p[4].x = p[11].x = -D / 2 + S;
    p[5].x = p[10].x = p[4].x + BEV;
    p[6].x = p[9].x = -p[5].x;
    p[7].x = p[8].x = -p[4].x;
    p[4].y = p[7].y = -E1 / 2 + S + BEV;
    p[5].y = p[6].y = p[4].y - BEV;
    p[8].y = p[11].y = -p[4].y;
    p[9].y = p[10].y = -p[5].y;
    /* L3, points 12..19 */
    height = A1 + A2 / 2 - MID / 2;
    for (i = 12; i < 20; ++i)
        p[i].z = height;
    p[12].x = -D / 2;
    p[12].y = -E1 / 2 + BEV;
    p[13].x = -D / 2 + BEV;
    p[13].y = p[12].y - BEV;
    p[14].x = -p[13].x;
    p[14].y = p[13].y;
    p[15].x = -p[12].x;
    p[15].y = p[12].y;
    p[16].x = p[15].x;
    p[16].y = -p[15].y;
    p[17].x = p[14].x;
    p[17].y = -p[14].y;
    p[18].x = p[13].x;
    p[18].y = p[17].y;
    p[19].x = p[12].x;
    p[19].y = p[16].y;
    /* L4, points 20 .. 27 */
    height += MID;
    for (i = 20; i < 28; ++i)
    {
        p[i].z = height;
        p[i].x = p[i - 8].x;
        p[i].y = p[i - 8].y;
    }
    /* L5, points 28 .. 29 */
    height = A1 + A2 - ND - BEV - BEV;
    p[28].z = p[29].z = height;
    off = (height - (A1 + (A2 + MID) / 2)) * slope;
    p[28].x = p[29].x = p[20].x + off;
    p[28].y = ndia + BEV;
    p[29].y = -p[28].y;
    /* L6, points 30..39 */
    height += BEV;
    for (i = 30; i < 40; ++i)
        p[i].z = height;
    off = (height - (A1 + (A2 + MID) / 2)) * slope;
    p[30].x = p[39].x = p[20].x + off + BEV;
    p[31].x = p[38].x = -D / 2 + NL - ndia;
    p[32].x = p[37].x = p[31].x + ndia * cos(3.0 / 8.0 * M_PI);
    p[33].x = p[36].x = p[31].x + ndia * cos(M_PI / 4.0);
    p[34].x = p[35].x = p[31].x + ndia * cos(M_PI / 8.0);
    p[30].y = p[31].y = ndia;
    p[32].y = ndia * sin(3.0 / 8.0 * M_PI);
    p[33].y = ndia * sin(M_PI / 4.0);
    p[34].y = ndia * sin(M_PI / 8.0);
    p[35].y = -p[34].y;
    p[36].y = -p[33].y;
    p[37].y = -p[32].y;
    p[38].y = p[39].y = -p[31].y;
    /* L7, points 40 .. 51 (notch), 52 .. 59 (rectangle) */
    height = A1 + A2 - BEV;
    for (i = 40; i < 60; ++i)
        p[i].z = height;
    p[40].x = p[51].x = -D / 2 + S;
    p[41].x = p[50].x = p[51].x + BEV;
    for (i = 42; i < 50; ++i)
    {
        p[i].x = p[i - 11].x;
        p[i].y = p[i - 11].y;
    }
    p[41].y = p[30].y;
    p[50].y = p[39].y;
    p[40].y = p[41].y + BEV;
    p[51].y = -p[40].y;
    for (i = 52; i < 60; ++i)
    {
        p[i].x = p[i - 48].x;
        p[i].y = p[i - 48].y;
    }
    /* L8, 60 .. 75 */
    height = A1 + A2;
    for (i = 60; i < 76; ++i)
        p[i].z = height;
    p[60].x = p[69].x = -D / 2 + S + BEV;
    p[61].x = p[68].x = -D / 2 + NL - ndia;
    ndia += BEV;
    p[62].x = p[67].x = p[61].x + ndia * cos(3.0 / 8.0 * M_PI);
    p[63].x = p[66].x = p[61].x + ndia * cos(M_PI / 4.0);
    p[64].x = p[65].x = p[61].x + ndia * cos(M_PI / 8.0);
    p[60].y = p[61].y = ndia;
    p[62].y = ndia * sin(3.0 / 8.0 * M_PI);
    p[63].y = ndia * sin(M_PI / 4.0);
    p[64].y = ndia * sin(M_PI / 8.0);
    p[69].y = p[68].y = -p[60].y;
    p[67].y = -p[62].y;
    p[66].y = -p[63].y;
    p[65].y = -p[64].y;
    p[70].x = p[75].x = -D / 2 + S + BEV;
    p[71].x = p[74].x = -D / 2 + NL + NW;
    p[72].x = p[73].x = -p[70].x;
    p[70].y = p[71].y = p[72].y = -E1 / 2 + S + BEV;
    p[73].y = p[74].y = p[75].y = -p[70].y;

    for (i = 0; i < CASE_NP; ++i)
        p[i] = p[i] * scale;
    valid = true;
    return p;
}

int dipcase::writeCoord(std::ofstream &fp, int tabs, Transform *t)
{
    int i;
    Quat loc;

    if (!valid)
        return -1;

    if (tabs < 0) tabs = 0;
    if (tabs > 4) tabs = 4;
    string fmt(4*tabs, ' ');

    fp << fmt << "coord Coordinate { point [\n";
    fp << fmt << "   ";
    for (i = 0; i < CASE_NP - 1; ++i)
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

int dipcase::writeFacets(std::ofstream &fp, int tabs)
{
    if (tabs < 0) tabs = 0;
    if (tabs > 4) tabs = 4;
    string fmt(4*tabs, ' ');

    fp << fmt << "coordIndex [\n";
    /* case, bottom */
    fp << fmt << "    0, 3, 2, 1, -1, 0, 5, 4, -1, 0, 1, 6, 5, -1, 1, 7, 6, -1,\n";
    fp << fmt << "    1, 2, 8, 7, -1, 2, 9, 8, -1, 2, 3, 10, 9, -1, 3, 11, 10, -1, 3, 0, 4, 11, -1,\n";
    /* case, lower bevel */
    fp << fmt << "    4, 12, 19, 11, -1, 5, 13, 12, 4, -1, 6, 14, 13, 5, -1, 7, 15, 14, 6, -1,\n";
    fp << fmt << "    8, 16, 15, 7, -1, 9, 17, 16, 8, -1, 10, 18, 17, 9, -1, 11, 19, 18, 10, -1,\n";
    /* case, middle */
    fp << fmt << "    13, 21, 20, 12, -1, 14, 22, 21, 13, -1, 15, 23, 22, 14, -1, 16, 24, 23, 15, -1,\n";
    fp << fmt << "    17, 25, 24, 16, -1, 18, 26, 25, 17, -1, 19, 27, 26, 18, 12, 20, 27, 19, -1,\n";
    /* case, upper bevel except front */
    fp << fmt << "    21, 53, 52, 20, -1, 22, 54, 53, 21, -1, 23, 55, 54, 22, -1, 24, 56, 55, 23, -1,\n";
    fp << fmt << "    25, 57, 56, 24, -1, 26, 58, 57, 25, -1, 27, 59, 58, 26, -1,\n";
    /* case, upper bevel front */
    fp << fmt << "    27, 28, 40, 59, -1, 20, 52, 51, 29, -1, 20, 29, 28, 27, -1,\n";
    /* case, notch, front bevel */
    fp << fmt << "    29, 51, 50, 39, -1, 28, 30, 41, 40, -1, 28, 29, 39, 30, -1,\n";
    /* case, notch, floor */
    fp << fmt << "    39, 38, 37, 36, 35, 34, 33, 32, 31, 30, -1,\n";
    /* case, notch, wall */
    fp << fmt << "    39, 50, 49, 38, -1, 38, 49, 48, 37, -1, 37, 48, 47, 36, -1, 36, 47, 46, 35, -1, 35, 46, 45, 34, -1,\n";
    fp << fmt << "    34, 45, 44, 33, -1, 33, 44, 43, 32, -1, 32, 43, 42, 31, -1, 31, 42, 41, 30, -1,\n";
    /* case, top bevel except front */
    fp << fmt << "    53, 70, 52, -1, 53, 54, 72, 70, -1, 54, 55, 72, -1, 55, 56, 73, 72, -1,\n";
    fp << fmt << "    56, 57, 73, -1, 57, 58, 75, 73, -1, 58, 59, 75, -1,\n";
    /* case, notch, top bevel */
    fp << fmt << "    59, 40, 60, 75, -1, 40, 41, 60, -1, 41, 42, 61, 60, -1, 42, 43, 62, 61, -1,\n";
    fp << fmt << "    43, 44, 63, 62, -1, 44, 45, 64, 63, -1, 45, 46, 65, 64, -1, 46, 47, 66, 65, -1, 47, 48, 67, 66, -1,\n";
    fp << fmt << "    48, 49, 68, 67, -1, 49, 50, 69, 68, -1, 50, 51, 69, -1, 51, 52, 70, 69, -1,\n";
    /* case, top */
    fp << fmt << "    68, 69, 70, 71, -1, 67, 68, 71, -1, 66, 67, 71, -1, 65, 66, 71, -1,\n";
    fp << fmt << "    74, 75, 60, 61, -1, 74, 61, 62, -1, 74, 62, 63, -1, 74, 63, 64, -1,\n";
    fp << fmt << "    64, 65, 71, 74, -1, 71, 72, 73, 74, -1\n" << fmt << "]\n";
    return fp.good() ? 0 : -1;
}
