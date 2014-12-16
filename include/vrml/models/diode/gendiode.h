/*
 *  file: gendiode.h
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

#ifndef GENDIODE_H
#define GENDIODE_H

#include <iosfwd>

#include <vrmlmat.h>

namespace KC3DDIODE
{
/**
 * Generic plastic encapsulated cylindrical diode
 */
class GENDIODE
{
private:
    bool hascol;        // true if the colors have been set
    KC3D::VRMLMAT wcol; // wire color
    KC3D::VRMLMAT bcol; // body color
    KC3D::VRMLMAT kcol; // cathode color

    int nwv;            // number of vertices on wire
    int ntv;            // number of vertices on tube
    int nb;             // number of segments in a 90 deg. bend

    bool hasparams;     // true when parameters have been set
    double dwire;       // wire diameter
    double dbody;       // body diameter
    double lbody;       // length of the body
    double wcath;       // cathode band's width
    double scath;       // distance from edge of diode to cathode band

public:
    GENDIODE();
    virtual ~GENDIODE();

    /**
     * Set the part material appearances
     * @param aWireMatFname [in] wire (lead) appearance
     * @param aBodyMatFname [in] body (tube) appearance
     * @param aCathodeMatFname [in] cathode marker appearance
     * @return 0 for success, -1 for failure
     */
    int SetColors( const std::string& aWireMatFname, const std::string& aBodyMatFname,
                   const std::string& aCathodeMatFname );

    /**
     * Set the number of vertices for each feature
     * @param aWireNVertices [in] number of vertices in wire cross-section
     * @param aTubeNVertices [in] number of vertices in tube cross-section
     * @param aBendNSegments [in] number of segments in a 90-deg bend
     * @return 0 for success, -1 for failure
     */
    int SetNVertices( int aWireNVertices, int aTubeNVertices, int aBendNSegments );

    /**
     * Set the general parameters
     * @param aWireDia [in] wire diameter
     * @param aBodyDia [in] body diameter
     * @param aBodyLength [in] length of the body
     * @param aMarkWidth [in] width of the cathode mark
     * @param aMarkGap [in] space from end of body to cathode band
     * @return 0 for success, -1 for failure
     */
    int SetParams( double aWireDia, double aBodyDia, double aBodyLength,
                   double aMarkWidth, double aMarkGap );

    /**
     * Write the diode model to the specified filename
     *
     * @param aPartname [in] name of the part; filename will be [partname].wrl
     * @param aScale [in] scale to apply to output (0.3937 to match KiCAD VRML expectations)
     * @param isHoriz [in] true if horizontal orientation
     * @param isVKFlipped [in] true if we wish to flip the anode/cathode in the vertical orientation
     * @param aPitch [in] lead pitch
     * @param aLeadDepth  [in] lead length below the top of PCB (default 2.0 mm)
     * @return 0 for success, -1 for failure
     */
    int Build( const std::string& aPartname, double aScale, bool isHoriz,
               bool isVKFlipped, double aPitch, double aLeadDepth = 2.0 );
};

}    // namespace KC3DDIODE
#endif // GENDIODE_H
