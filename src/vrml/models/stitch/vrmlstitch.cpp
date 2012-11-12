/*
 *      file: stitchvrml.cpp
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

/**
 *  \brief This program takes a list of VRML parts and transforms and produces
 *  an assembly compatible with the KiCAD 3D Viewer.
 *
 *  Usage: vrmlstitch <input_file>
 *
 *  Input format:
 *  #           =   comment
 *  file:       =   output basename specification
 *  grot:       =   global rotation parameters N X Y Z; N = coefficient to PI
 *                  for example 0.5, 0, 0, 1 is a 90-degree rotation about the Z axis
 *  gtr:        =   global translation parameters X Y Z
 *  gsc:        =   global scaling parameters X Y Z
 *  .begin      =   start of a part information block
 *  name:       =   path to VRML file
 *  r:          =   rotation matrix N X Y Z to apply to model
 *  t:          =   X Y Z translation to apply to model
 *  s:          =   X Y Z scale to apply to model
 *  .end        =   end of a part information block
 *
 *  The input file may contain multiple .begin .. .end blocks, 1 for each
 *  component in the assembly.
 */

/*
 *  Note: The rotations are about an axis which passes through (0,0,0) by default.
 *      The order of operations in a transform is:
 *          (1) Rotation
 *          (2) Translation
 *          (3) Scale
 *
 *      The rotation matrix, if it appears in a VRML Transform{} block,
 *      must be normalized or else the scale is distorted.
 *
 *  The Scheme:
 *
 *      + Since we want this assembly to contain no external links, all files
 *      are read and rewritten into a single assembly file.
 *
 *      + All comments are dropped
 *
 *      + All components are placed within a single Transform block to ensure that
 *          viewers such as WhiteDune do not rotate or move portions of the assembly
 *          independently.
 *
 *      + Since the KiCAD 3D viewer does not handle actual transformations,
 *          identical parts must all be repeated. If the KiCAD viewer handled
 *          transformations sensibly we could save an awful lot of space by
 *          taking advantage of the DEF feature.
 *
 *      + Every line is inspected for the "coord Coordinate {" phrase
 *          and all subsequent numbers are transformed until the end of the
 *          Coordinate { block. Within the block streams need to be parsed by
 *          each character; non-numeric characters are simply passed on while
 *          numeric characters are interpreted, put into triplets, and transformed
 *          before writing out.
 *
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>

#include "vdefs.h"
#include "transform.h"
#include "vcom.h"

using namespace std;
using namespace kc3d;


void printUsage(void)
{
    cerr << "\n***\n";
    cerr << "Usage: vrmlstitch <input_file>\n";
    cerr << "Input file consists of an output file specification and\n";
    cerr << "optional global transformation parameters followed by\n";
    cerr << "multiple blocks of part information with the form:\n";
    cerr << "\t.begin\n";
    cerr << "\tname: <VRML model filename>\n";
    cerr << "\tr: <N X Y Z> where N is a coefficient for PI and X Y Z are the orientation vector\n";
    cerr << "\ts: <X Y Z> scale factors for X Y Z axes\n";
    cerr << "\tt: <X Y Z> translation along X Y Z axes\n";
    cerr << "\t.end\n";
    cerr << "The input file may also have comment lines which begin with '#'\n";
    cerr << "The resulting component can be reoriented by specifying the following\n";
    cerr << "global parameters before the first block entry:\n";
    cerr << "\tgrot: <N X Y Z>  global rotation\n";
    cerr << "\tgtr:  <X Y Z>  global translation\n";
    cerr << "\tgsc:  <X Y Z>  global scale factor\n";
    cerr << "The global transformation parameters are applied after the\n";
    cerr << "block-specific transformations.\n";
    cerr << "The output file basename is specified by a 'file:' parameter\n";
    cerr << "which precedes the part information, for example:\n";
    cerr << "\tfile: some_output_file\n";
    cerr << "***\n\n";
    return;
}

struct INFO
{
    std::string name;
    Transform   tx;
    int ln;             // line number

    INFO()
    {
        ln = 0;
        return;
    }
};

struct OFS
{
    int ofs;
    int nest;
    double x;
    double y;
    double z;
    OFS()
    {
        ofs = 0;
        nest = 0;
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }
};

// read the output filename and global transform parameters
int getGlobs(std::ifstream &file, INFO &glob);
// parse the line for a Key: Value pair
int getKeys(const std::string &line, std::string &key, std::string &val);
// parse the string for N, X, Y, Z values and set up the rotation
int getRot(const std::string &val, Rotation &rot);
// parse the string for X, Y, Z values
int getXYZ(const std::string &val, double *x, double *y, double *z);
// read the part information
int readInfo(std::ifstream &file, INFO &glob, INFO &info);
// process the input file
int process(std::ofstream &ofile, INFO &glob, INFO &part);
// search for the next ".begin" line
int nextPart(std::ifstream &file, INFO &glob);
// create the part name from the filename
const std::string &getPartName(const std::string &name);
// parse the input line and write data to the output file
int parsePart(std::ofstream &ofile, INFO &glob,
        const std::string &line, INFO &part, OFS &ofs);


int main (int argc, const char **argv)
{
    if (argc < 2)
    {
        printUsage();
        return 0;
    }

    ifstream ifile;
    ofstream ofile;
    INFO glob;
    INFO part;

    ifile.open(argv[1]);
    if (!ifile.is_open())
    {
        ERRBLURB;
        cerr << "could not open file: '" << argv[1] << "'\n";
        return -1;
    }

    // fetch
    if (getGlobs(ifile, glob)) return -1;

    string ofname = glob.name;
    ofname += ".wrl";
    if (SetupVRML(ofname, ofile))
    {
        ERRBLURB;
        cerr << "could not open output file: '" << glob.name << "'\n";
        return -1;
    }

    string partname = getPartName(glob.name);
    // set up an encompassing Transform to ensure that other viewers
    // do not mangle the appearance
    cerr << "partname: " << partname << "\n";
    SetupXForm(partname, ofile, 0);

    // loop:
    //      read VRML part information
    //      process VRML file
    //      search for next VRML part block
    while ((ifile.good()) && (!ifile.eof()))
    {
        if (readInfo(ifile, glob, part))
        {
            ofile.close();
            return -1;
        }
        if (process(ofile, glob, part))
        {
            ofile.close();
            return -1;
        }
        if (nextPart(ifile, glob)) break;
    }

    CloseXForm(ofile, 0);
    ofile.close();

    return 0;
}   // int main()



int getGlobs(std::ifstream &file, INFO &glob)
{
    if (!file.is_open())
    {
        ERRBLURB;
        cerr << "input file not opened\n";
        return -1;
    }
    string line;
    string key;
    string val;
    double tx, ty, tz;
    Rotation grot;
    Translation gtr;
    Scale gsc;
    int ifs = 0;
    bool hn, hr, ht, hs;
    hn = hr = ht = hs = false;

    glob.name.clear();
    while ((file.good()) && (!file.eof()))
    {
        line.clear();
        getline(file, line);
        ++glob.ln;
        if (line.empty()) continue;
        if (line.c_str()[0] == '#') continue;
        if (!line.compare(0, 6, ".begin"))
        {
            ifs = 1;
            break;
        }
        if (!line.compare(0, 4, ".end"))
        {
            ERRBLURB;
            cerr << "ill-formed input file; unexpected '.end' at line #" << glob.ln << "\n";
            return -1;
        }
        if (!getKeys(line, key, val))
        {
            // process the key: value pair
            // if not one of 'file', 'grot', 'gtr', 'gsc'
            // we have an ill-formed input file
            if (!key.compare("file"))
            {
                if (hn)
                {
                    ERRBLURB;
                    cerr << "ill-formed input at line #" << glob.ln << "\n";
                    cerr << "multiple output filename specifications\n";
                    return -1;
                }
                hn = true;
                glob.name = val;
                continue;
            }
            if (!key.compare("grot"))
            {
                if (hr)
                {
                    ERRBLURB;
                    cerr << "ill-formed input at line #" << glob.ln << "\n";
                    cerr << "multiple global rotation specifications\n";
                    return -1;
                }
                hr = true;
                if (getRot(val, grot))
                {
                    ERRBLURB;
                    cerr << "ill-formed input at line #" << glob.ln << "\n";
                    return -1;
                }
                continue;
            }
            if (!key.compare("gtr"))
            {
                if (ht)
                {
                    ERRBLURB;
                    cerr << "ill-formed input at line #" << glob.ln << "\n";
                    cerr << "multiple global translation specifications\n";
                    return -1;
                }
                ht = true;
                if (getXYZ(val, &tx, &ty, &tz))
                {
                    ERRBLURB;
                    cerr << "ill-formed input at line #" << glob.ln << "\n";
                    return -1;
                }
                gtr.set(tx, ty, tz);
                continue;
            }
            if (!key.compare("gsc"))
            {
                if (hs)
                {
                    ERRBLURB;
                    cerr << "ill-formed input at line #" << glob.ln << "\n";
                    cerr << "multiple global scaling specifications\n";
                    return -1;
                }
                hs = true;
                if (getXYZ(val, &tx, &ty, &tz))
                {
                    ERRBLURB;
                    cerr << "ill-formed input at line #" << glob.ln << "\n";
                    return -1;
                }
                gsc.set(tx, ty, tz);
                continue;
            }
            ERRBLURB;
            cerr << "ill-formed input at line #" << glob.ln << "\n";
            cerr << "unexpected key: '" << key << "'\n";
            return -1;
        }
        else
        {
            ERRBLURB;
            cerr << "ill-formed input file at line #" << glob.ln << "\n";
            return -1;
        }
    } // loop reading global params

    if (ifs != 1)
    {
        ERRBLURB;
        cerr << "no '.begin' line found in input file\n";
        return -1;
    }
    if (glob.name.empty())
    {
        ERRBLURB;
        cerr << "no output file basename specified\n";
        return -1;
    }
    glob.tx.setTranslation(gtr);
    glob.tx.setRotation(grot);
    glob.tx.setScale(gsc);

    return 0;
}   // int getGlobs()



int getKeys(const std::string &line, std::string &key, std::string &val)
{
    string::const_iterator sp = line.begin();
    string::const_iterator ep = line.end();

    key.clear();
    val.clear();

    while(sp != ep)
    {
        if (isspace(*sp))
        {
            ++sp;
            continue;
        }
        if (*sp == ':')
        {
            ++sp;
            break;
        }
        key.push_back(*sp);
        ++sp;
    }
    if (key.empty())
    {
        ERRBLURB;
        cerr << "no key found in line '" << line << "'\n";
        return -1;
    }

    while(sp != ep)
    {
        if (isspace(*sp)&&(val.empty()))
        {
            ++sp;
            continue;
        }
        val.push_back(*sp);
        ++sp;
    }
    if (val.empty())
    {
        ERRBLURB;
        cerr << "no value found for key '" << key << "'\n";
        return -1;
    }
    // trim space from right
    string::iterator msp = val.begin();
    string::iterator mep = val.end();

    --mep;
    while ((mep != msp) && (isspace(*mep))) val.erase(mep--);

    return 0;
}   // int getKeys()



int getRot(const std::string &val, Rotation &rot)
{
    istringstream istr(val);
    double ang = -1.0e+20;
    double tx = -1.0e+20;
    double ty = -1.0e+20;
    double tz = -1.0e+20;

    istr >> ang >> tx >> ty >> tz;
    if ((ang < -0.9e+20) || (tx < -0.9e+20) || (ty < -0.9e+20) || (tz < -0.9e+20))
    {
        ERRBLURB;
        cerr << "\tcould not parse R X Y Z values\n";
        return -1;
    }
    if ((ang < -2.0) || (ang > 2.0))
    {
        ERRBLURB;
        cerr << "\tinvalid angle coefficient (" << setprecision(12) << ang << "); valid range is -2.0 .. 2.0\n";
        return -1;
    }
    ang *= M_PI;
    rot.set(ang, tx, ty, tz);

    return 0;
}   // int getRot()



int getXYZ(const std::string &val, double *x, double *y, double *z)
{
    istringstream istr(val);
    float tx = -1.0e+20;
    float ty = -1.0e+20;
    float tz = -1.0e+20;

    istr >> tx >> ty >> tz;
    if ((tx < -0.9e+20) || (ty < -0.9e+20) || (tz < -0.9e+20))
    {
        ERRBLURB;
        cerr << "\tcould not parse X Y Z values\n";
        return -1;
    }
    *x = tx;
    *y = ty;
    *z = tz;

    return 0;
}   // int getXYZ



// read the part information
int readInfo(std::ifstream &file, INFO &glob, INFO &info)
{
    if (!file.is_open())
    {
        ERRBLURB;
        cerr << "input file not opened\n";
        return -1;
    }
    string line;
    string key;
    string val;
    double tx, ty, tz;
    Rotation lrot;
    Translation ltr;
    Scale lsc;
    int ifs = 0;
    bool hn, hr, ht, hs;
    hn = hr = ht = hs = false;

    info.ln = 0;
    info.name.clear();
    info.tx.set(ltr, lrot, lsc);
    while ((file.good()) && (!file.eof()))
    {
        line.clear();
        getline(file, line);
        ++glob.ln;
        if (line.empty()) continue;
        if (line.c_str()[0] == '#') continue;
        if (!line.compare(0, 6, ".begin"))
        {
            ERRBLURB;
            cerr << "ill-formed input file; unexpected '.begin' at line #" << glob.ln << "\n";
            return -1;
        }
        if (!line.compare(0, 4, ".end"))
        {
            ifs = 1;
            break;
        }
        if (!getKeys(line, key, val))
        {
            // process the key: value pair
            // if not one of 'name', 'r', 't', 's'
            // we have an ill-formed input file
            if (!key.compare("name"))
            {
                if (hn)
                {
                    ERRBLURB;
                    cerr << "ill-formed input at line #" << glob.ln << "\n";
                    cerr << "multiple part filename specifications\n";
                    return -1;
                }
                hn = true;
                info.name = val;
                continue;
            }
            if (!key.compare("r"))
            {
                if (hr)
                {
                    ERRBLURB;
                    cerr << "ill-formed input at line #" << glob.ln << "\n";
                    cerr << "multiple part rotation specifications\n";
                    return -1;
                }
                hr = true;
                if (getRot(val, lrot))
                {
                    ERRBLURB;
                    cerr << "ill-formed input at line #" << glob.ln << "\n";
                    return -1;
                }
                continue;
            }
            if (!key.compare("t"))
            {
                if (ht)
                {
                    ERRBLURB;
                    cerr << "ill-formed input at line #" << glob.ln << "\n";
                    cerr << "multiple part translation specifications\n";
                    return -1;
                }
                ht = true;
                if (getXYZ(val, &tx, &ty, &tz))
                {
                    ERRBLURB;
                    cerr << "ill-formed input at line #" << glob.ln << "\n";
                    return -1;
                }
                ltr.set(tx, ty, tz);
                continue;
            }
            if (!key.compare("s"))
            {
                if (hs)
                {
                    ERRBLURB;
                    cerr << "ill-formed input at line #" << glob.ln << "\n";
                    cerr << "multiple part scaling specifications\n";
                    return -1;
                }
                hs = true;
                if (getXYZ(val, &tx, &ty, &tz))
                {
                    ERRBLURB;
                    cerr << "ill-formed input at line #" << glob.ln << "\n";
                    return -1;
                }
                lsc.set(tx, ty, tz);
                continue;
            }
            ERRBLURB;
            cerr << "ill-formed input at line #" << glob.ln << "\n";
            cerr << "unexpected key: '" << key << "'\n";
            return -1;
        }
        else
        {
            ERRBLURB;
            cerr << "ill-formed input file at line #" << glob.ln << "\n";
            return -1;
        }
    } // loop reading global params

    if (ifs != 1)
    {
        ERRBLURB;
        cerr << "no '.end' line to terminate a part information block\n";
        return -1;
    }
    if (info.name.empty())
    {
        ERRBLURB;
        cerr << "no part file specified\n";
        return -1;
    }
    info.tx.setTranslation(ltr);
    info.tx.setRotation(lrot);
    info.tx.setScale(lsc);

    return 0;
}   // int readInfo()



// search for the next ".begin" line
int nextPart(std::ifstream &file, INFO &glob)
{
    if (!file.is_open())
    {
        ERRBLURB;
        cerr << "input file not opened\n";
        return -1;
    }
    string line;

    while ((file.good()) && (!file.eof()))
    {
        line.clear();
        getline(file, line);
        ++glob.ln;
        if (line.empty()) continue;
        if (line.c_str()[0] == '#') continue;
        if (!line.compare(0, 6, ".begin")) return 0;
        ERRBLURB;
        cerr << "ill-formed input file; unexpected text at line #" << glob.ln << "\n";
        return -1;
    }
    return -1;
}   // int nextPart()



// create the part name from the filename
const std::string &getPartName(const std::string &name)
{
    static string bname;
    bname = "ASSY_";
    char ic;
    std::string::const_iterator start = name.begin();
    std::string::const_iterator end = name.end();

    while (start != end)
    {
        ic = *start;
        ++start;
        // only accept alphanumerics and underscores;
        // convert dots to underscores
        if ((!isalnum(ic)) && (ic != '_'))
        {
            if (ic != '.') continue;
            ic = '_';
        }
        ic = toupper(ic);
        bname += ic;
    }
    return bname;
}   // const std::string& getPartName()



// process the input file
int process(std::ofstream &ofile, INFO &glob, INFO &part)
{
    string line;
    ifstream file;
    file.open(part.name.c_str());
    if (!file.is_open())
    {
        ERRBLURB;
        cerr << "could not open part file: '" << part.name << "'\n";
        return -1;
    }
    OFS ofs;

    part.ln = 0;
    while ((file.good()) && (!file.eof()))
    {
        line.clear();
        getline(file, line);
        ++part.ln;
        if (line.empty())
        {
            ofile << "\n";
            continue;
        }
        if (line.c_str()[0] == '#') continue;   // skip all comments
        if (parsePart(ofile, glob, line, part, ofs)) return -1;
    }
    if (!ofile.good())
    {
        ERRBLURB;
        cerr << "problems writing output file\n";
        return -1;
    }

    return 0;
}   // int process()


// parse the input line and write data to the output file
int parsePart(std::ofstream &ofile, INFO &glob,
        const std::string &line, INFO &part, OFS &fs)
{
    if (!fs.ofs)
    {
        if (line.find("Coordinate") == string::npos)
        {
            ofile << line << "\n";
            if (!ofile.good())
            {
                ERRBLURB;
                cerr << "problems writing to output file\n";
                return -1;
            }
            return 0;
        }
        fs.ofs = 1;
    }

    string::const_iterator start = line.begin();
    string::const_iterator end = line.end();
    string out;
    char oc, lc;

    /*
     *  Parser states
     *      0: no special processing; we may write out entire lines
     *      1: check each character and classify as one of
     *          {, space, alpha, digit, decimal point
     *      2: building up triplet0; accept only digits and dots
     *          until a space is encountered
     *      3: expect spaces until dot or digit for triplet 1
     *      4: building up triplet1
     *      5: expect spaces until dot or digit for triplet 2
     *      6. building up triplet2
     */

    lc = 0;
    while (start != end)
    {
        oc = *start++;
        switch (fs.ofs)
        {
        case 0:
            ERRBLURB;
            cerr << "BUG: unexpected code branch\n";
            break;
        case 1:
            /*
             *  1: check each character and classify as one of
             *      {, space, alpha, digit, decimal point
             */
            if (oc == '{') ++fs.nest;
            if (oc == '}')
            {
                --fs.nest;
                if (fs.nest < 0)
                {
                    ERRBLURB;
                    cerr << "BUG: OFS.nest < 0 at line " << part.ln << "\n";
                    cerr << "\tinput file: " << part.name << "\n";
                    return -1;
                }
                if (!fs.nest)
                {
                    out += oc;
                    while (start != end) out += *start++;
                    ofile << out << "\n";
                    if (!ofile.good())
                    {
                        ERRBLURB;
                        cerr << "problems writing to output file\n";
                        return -1;
                    }
                    fs.ofs = 0;
                    return 0;
                }
            }
            if ((isdigit(oc)) || (oc == '.') || (oc == '+') || (oc == '-'))
            {
                if (!isalpha(lc))
                {
                    ofile << out;
                    if (!ofile.good())
                    {
                        ERRBLURB;
                        cerr << "problems writing output file\n";
                        return -1;
                    }
                    out.clear();
                    fs.ofs = 2; // start of the first triplet
                }
            }
            // let all other characters filter through
            out += oc;
            lc = oc;
            continue;
            break;
        case 2:
            /*
             *  2: building up triplet0; accept only digits and dots
             *      until a space is encountered
             */
            if (isspace(oc))
            {
                istringstream os;
                os.str(out);
                os >> fs.x;
                out.clear();
                fs.ofs = 3;     // eat spaces in anticipation of triplet1
                lc = ' ';
                continue;
            }
            if ((!isdigit(oc)) && (oc != '.') && (oc != '+') && (oc != '-')
                    && (oc != 'e') && (oc != 'E'))
            {
                ERRBLURB;
                cerr << "problems parsing triplet0 for transformation at line " << part.ln << "\n";
                cerr << "\tinput file: " << part.name << "\n";
                return -1;
            }
            out += oc;
            lc = oc;
            break;
        case 3:
            /*  3: expect spaces until dot or digit for triplet 1 */
            if (isspace(oc)) continue;
            if ((isdigit(oc)) || (oc == '.') || (oc == '+') || (oc == '-'))
            {
                out += oc;
                lc = oc;
                fs.ofs = 4;
                continue;
            }
            ERRBLURB;
            cerr << "unexpected character at line " << part.ln << "\n";
            cerr << "\tinput file: " << part.name << "\n";
            return -1;
            break;
        case 4:
            /*
             *  4: building up triplet1; accept only digits and dots
             *      until a space is encountered
             */
            if (isspace(oc))
            {
                istringstream os;
                os.str(out);
                os >> fs.y;
                out.clear();
                fs.ofs = 5;     // eat spaces in anticipation of triplet2
                lc = ' ';
                continue;
            }
            if ((!isdigit(oc)) && (oc != '.') && (oc != '+') && (oc != '-')
                    && (oc != 'e') && (oc != 'E'))
            {
                ERRBLURB;
                cerr << "problems parsing triplet1 for transformation at line " << part.ln << "\n";
                cerr << "\tinput file: " << part.name << "\n";
                return -1;
            }
            out += oc;
            lc = oc;
            break;
        case 5:
            /*  5: expect spaces until dot or digit for triplet 2 */
            if (isspace(oc)) continue;
            if ((isdigit(oc)) || (oc == '.') || (oc == '+') || (oc == '-'))
            {
                out += oc;
                lc = oc;
                fs.ofs = 6;
                continue;
            }
            ERRBLURB;
            cerr << "unexpected character at line " << part.ln << "\n";
            cerr << "\tinput file: " << part.name << "\n";
            return -1;
            break;
        case 6:
            /* 6: building up triplet2 */
            if ((!isdigit(oc)) && (oc != '.') && (oc != '+') && (oc != '-')
                    && (oc != 'e') && (oc != 'E'))
            {
                istringstream os;
                os.str(out);
                os >> fs.z;
                out.clear();
                // write out the transformed triplet
                part.tx.transform(&fs.x, &fs.y, &fs.z, 1);
                glob.tx.transform(&fs.x, &fs.y, &fs.z, 1);
                if ((fs.x < 1e-9) && (fs.x > 11e-9)) fs.x = 0.0;
                if ((fs.y < 1e-9) && (fs.y > 11e-9)) fs.y = 0.0;
                if ((fs.z < 1e-9) && (fs.z > 11e-9)) fs.z = 0.0;
                ofile << setprecision(8) << fs.x << " " << fs.y << " " << fs.z;
                if (!ofile.good())
                {
                    ERRBLURB;
                    cerr << "problems writing output file\n";
                    return -1;
                }
                out += oc;
                fs.ofs = 1;
                lc = 0;
                if (oc == '}')
                {
                    --fs.nest;
                    if (!fs.nest)
                    {
                        while (start != end) out += *start++;
                        ofile << out << "\n";
                        fs.ofs = 0;
                        if (!ofile.good())
                        {
                            ERRBLURB;
                            cerr << "problems writing output file\n";
                            return -1;
                        }
                        return 0;
                    }
                }
                continue;
            }
            out += oc;
            lc = oc;
            break;
        }
    }   // while (start != end)

    switch (fs.ofs)
    {
    case 2:
        do
        {
            istringstream os;
            os.str(out);
            os >> fs.x;
            out.clear();
            fs.ofs = 3;     // eat spaces in anticipation of triplet1
        } while (0);
        break;
    case 4:
        do
        {
            istringstream os;
            os.str(out);
            os >> fs.y;
            out.clear();
            fs.ofs = 5;     // eat spaces in anticipation of triplet1
        } while (0);
        break;
    case 6:
        do
        {
            istringstream os;
            os.str(out);
            os >> fs.z;
            fs.ofs = 1;
            part.tx.transform(&fs.x, &fs.y, &fs.z, 1);
            glob.tx.transform(&fs.x, &fs.y, &fs.z, 1);
            if ((fs.x < 1e-9) && (fs.x > 11e-9)) fs.x = 0.0;
            if ((fs.y < 1e-9) && (fs.y > 11e-9)) fs.y = 0.0;
            if ((fs.z < 1e-9) && (fs.z > 11e-9)) fs.z = 0.0;
            ofile << setprecision(8) << fs.x << " " << fs.y << " " << fs.z;
            if (!ofile.good())
            {
                ERRBLURB;
                cerr << "problems writing output file\n";
                return -1;
            }
        } while (0);
        break;
    default:
        if (!out.empty())
        {
            ofile << out << "\n";
            if (!ofile.good())
            {
                ERRBLURB;
                cerr << "problems writing output file\n";
                return -1;
            }
        }
        break;
    }

    return 0;
}   // int parsePart()
