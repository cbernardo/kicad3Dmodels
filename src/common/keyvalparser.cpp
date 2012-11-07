/*
 *      file: keyvalparser.cpp
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
#include <fstream>
#include <cctype>

#include "keyvalparser.h"

using namespace std;
using namespace kc3d;

KeyValParser::~KeyValParser()
{
    keyvals.clear();
    return;
}

int KeyValParser::LoadKeys(const std::string &filename)
{
    ifstream file;
    string iline;

    file.open(filename.c_str());
    if (!file.is_open())
    {
        cerr << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "(): could not open file \""
                << filename << "\".\n";
        return -1;
    }

    while (file.good()&&!file.eof())
    {
        getline(file, iline);
        parseLine(iline);
    }
    if (keyvals.empty())
    {
        cout << "No key:value pairs found in file: " << filename << "\n";
    }

    return keyvals.size();
}

void KeyValParser::ClearKeys(void)
{
    keyvals.clear();
    return;
}

const std::map<std::string, std::string> &
KeyValParser::GetKeys(void)
{
    return keyvals;
}


int KeyValParser::parseLine(const std::string &line)
{
    if (line.empty()) return 0;
    if (!line.compare(0,1,"#",1)) return 0;

    string key;
    string val;

    string::const_iterator sp = line.begin();
    string::const_iterator ep = line.end();

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
    if (key.empty()) return 0;

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
    if (val.empty()) return 0;
    // trim space from right
    string::iterator msp = val.begin();
    string::iterator mep = val.end();

    --mep;
    while ((mep != msp) && (isspace(*mep))) val.erase(mep--);

    keyvals.insert(pair<string, string> (key, val));

    return 1;
}
