/*
 *      file: keyvalparser.cpp
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

#include <iostream>
#include <fstream>
#include <cctype>

#include <vdefs.h>
#include <keyvalparser.h>

using namespace std;
using namespace KC3D;

KEYVAL_PARSER::~KEYVAL_PARSER()
{
    keyvals.clear();
}


int KEYVAL_PARSER::LoadKeys( const std::string& aFilename )
{
    ifstream file;
    string iline;

    file.open( aFilename.c_str() );

    if( !file.is_open() )
    {
        ERRBLURB << "could not open file \"" << aFilename << "\".\n";
        return -1;
    }

    while( file.good()&&!file.eof() )
    {
        getline( file, iline );
        parseLine( iline );
    }

    if( keyvals.empty() )
    {
        cout << "* Warning: No key:value pairs found in file: " << aFilename << "\n";
    }

    return keyvals.size();
}


void KEYVAL_PARSER::ClearKeys( void )
{
    keyvals.clear();
}


const std::map<std::string, std::string>& KEYVAL_PARSER::GetKeys( void )
{
    return keyvals;
}


int KEYVAL_PARSER::parseLine( const std::string& aLine )
{
    if( aLine.empty() )
        return 0;

    if( !aLine.compare( 0, 1, "#", 1 ) )
        return 0;

    string key;
    string val;

    string::const_iterator sp   = aLine.begin();
    string::const_iterator ep   = aLine.end();

    while( sp != ep )
    {
        if( isspace( *sp ) )
        {
            ++sp;
            continue;
        }

        if( *sp == ':' )
        {
            ++sp;
            break;
        }

        key.push_back( *sp );
        ++sp;
    }

    if( key.empty() )
        return 0;

    while( sp != ep )
    {
        if( isspace( *sp )&&( val.empty() ) )
        {
            ++sp;
            continue;
        }

        val.push_back( *sp );
        ++sp;
    }

    if( val.empty() )
        return 0;

    // trim space from right
    string::iterator msp    = val.begin();
    string::iterator mep    = val.end();

    --mep;

    while( (mep != msp) && ( isspace( *mep ) ) )
        val.erase( mep-- );

    keyvals.insert( pair<string, string> ( key, val ) );

    return 1;
}
