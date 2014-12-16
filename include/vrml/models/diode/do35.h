/*
 *  file: do35.h
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
 */

#ifndef DO35_H
#define DO35_H

#include <iosfwd>

#include <vrmlmat.h>

namespace KC3DDIODE
{
/**
 * DO35 glass encapsulated package
 * such as a 1N4148 diode.
 */
class DO35
{
private:
    bool hascol;        // true if the colors have been set
    KC3D::VRMLMAT wcol; // wire color
    KC3D::VRMLMAT gcol; // glass color
    KC3D::VRMLMAT kcol; // cathode color
    KC3D::VRMLMAT tcol; // inner tube color

    int nwv;            // number of vertices on wire
    int ntv;            // number of vertices on tube
    int nb;             // number of segments in a 90 deg. bend

public:
    DO35();
    virtual ~DO35();

    /**
     * Set the part material appearances
     * @param aWireMatFname [in] wire (lead) appearance
     * @param aGlassMatFname [in] body (glass tube) appearance
     * @param aCathodeMatFname [in] cathode marker appearance
     * @param aInnerTubeMatFname [in] inner (copper tube) appearance
     * @return 0 for success, -1 for failure
     */
    int SetColors( const std::string& aWireMatFname, const std::string& aGlassMatFname,
                   const std::string& aCathodeMatFname, const std::string& aInnerTubeMatFname );

    /**
     * Set the number of vertices for each feature
     * @param aWireNVertices [in] number of vertices in wire cross-section
     * @param aTubeNVertices [in] number of vertices in tube cross-section
     * @param aBendNSegments [in] number of segments in a 90-deg bend
     * @return 0 for success, -1 for failure
     */
    int SetNVertices( int aWireNVertices, int aTubeNVertices, int aBendNSegments );

    /**
     * Write the D0-35 model to the specified filename
     *
     * @param aPartname [in] name of the part; filename will be [partname].wrl
     * @param aScale [in] scale to apply to output (0.3937 to match KiCAD VRML expectations)
     * @param isHoriz [in] true if horizontal orientation
     * @param isVKFlipped [in] true if we wish to flip the anode/cathode in the vertical orientation
     * @param aPitch [in] lead pitch
     * @param aLeadDepth  [in] lead length below the top of PCB (default 2.0 mm)
     * @return
     */
    int Build( const std::string& aPartname, double aScale, bool isHoriz,
            bool isVKFlipped, double aPitch, double aLeadDepth = 2.0 );
};


}   // namespace KC3DDIODE
#endif  // DO35_H
