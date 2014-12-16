/*
 *      file: keyvalparser.h
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

/**
 * \file
 * \defgroup common_tools Common Tools
 */


#ifndef KEYVALPARSER_H
#define KEYVALPARSER_H

#include <string>
#include <map>

namespace KC3D {
/**
 *      \ingroup common_tools
 *      \brief Key:Value parser
 *
 *      Class KEYVAL_PARSER reads a file and extracts
 *      key:value pairs. Lines beginning with '#' are
 *      ignored. Spaces within a key name are discarded;
 *      leading and trailing spaces within key names and
 *      values are discarded.
 */
class KEYVAL_PARSER
{
private:
    std::map<std::string, std::string> keyvals;
    int parseLine( const std::string& aLine );

public:
    virtual ~KEYVAL_PARSER();

    /**
     * Load Key:Value pairs from a file
     *
     * @param aFilename [in] file to open and read from
     * @return Number of key:value pairs found or -1 for failure
     */
    int LoadKeys( const std::string& aFilename );

    /**
     * Clear all current key:value data
     */
    void ClearKeys( void );

    /**
     * Retrieve the key:value data
     *
     * @return key:value data as paired strings in a map
     */
    const std::map<std::string, std::string>& GetKeys( void );
};
}       // namespace KC3D

#endif  // KEYVALPARSER_H
