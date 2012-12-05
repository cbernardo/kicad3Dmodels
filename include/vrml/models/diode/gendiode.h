/*
 file: gendiode.h

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

 */

#ifndef GENDIODE_H_
#define GENDIODE_H_

#include <iosfwd>

#include "vrmlmat.h"

namespace kc3ddiode
{

/**
 * Generic plastic encapsulated cylindrical diode
 */
class GenDiode
{
private:
    bool hascol;    // true if the colors have been set
    kc3d::VRMLMat wcol;   // wire color
    kc3d::VRMLMat bcol;   // body color
    kc3d::VRMLMat kcol;   // cathode color

    int nwv;        // number of vertices on wire
    int ntv;        // number of vertices on tube
    int nb;         // number of segments in a 90 deg. bend

    bool hasparams; // true when parameters have been set
    double dwire;   // wire diameter
    double dbody;   // body diameter
    double lbody;   // length of the body
    double wcath;   // cathode band's width
    double scath;   // distance from edge of diode to cathode band

public:
    GenDiode();
    virtual ~GenDiode();

    int SetColors(const std::string &wirecolor, const std::string &bodycolor,
            const std::string &cathodecolor);

    int SetNVertices(int wire, int tube, int bend);

    /**
     * Set the general parameters
     * @param wire [in] wire diameter
     * @param body [in] body diameter
     * @param length [in] length of the body
     * @param body [in] body diameter
     * @param space [in] space from end of body to cathode band
     * @return 0 for success, -1 for failure
     */
    int SetParams(double wire, double body, double length, double band, double space);

    /**
     * Write the diode model to the specified filename
     *
     * @param partname [in] name of the part; filename will be [partname].wrl
     * @param scale [in] scale to apply to output (0.3937 to match KiCAD VRML expectations)
     * @param horiz [in] true if horizontal orientation
     * @param vflip [in] true if we wish to flip the anode/cathode in the vertical orientation
     * @param pitch [in] lead pitch
     * @param lead  [in] lead length below the top of PCB (default 2.0 mm)
     * @return 0 for success, -1 for failure
     */
    int Build(const std::string &partname, double scale , bool horiz, bool vflip, double pitch, double lead = 2.0);

};

} /* namespace kc3d */
#endif /* GENDIODE_H_ */
