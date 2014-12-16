/*
 *      file: hdrtest.cpp
 *
 *      Copyright 2012-2014 Cirilo Bernardo (cjh.bernardo@gmail.com)
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
#include <string>

#include <vdefs.h>
#include <vcom.h>
#include <transform.h>
#include <vrmlmat.h>
#include <hdrbase.h>
#include <genhdr.h>
#include <k3dconfig.h>

using namespace std;
using namespace KC3D;
using namespace KC3DCONN;

int testHdrbase( void );

int main()
{
    GENHDR hdr;
    TRANSFORM   t0;
    ofstream    out;

    /*
     *  hdr.SetColors(K3D_TOP_DIR "/mcad/colors/black.mat", K3D_TOP_DIR "/mcad/colors/tin.mat",
     *       K3D_TOP_DIR "/mcad/colors/gold.mat");
     *  hdr.SetColors(K3D_TOP_DIR "/mcad/colors/rbc_crm_g.mat", K3D_TOP_DIR "/mcad/colors/tin.mat",
     *       K3D_TOP_DIR "/mcad/colors/gold.mat");
     */

    hdr.SetColors( K3D_TOP_DIR "/mcad/colors/rbc_crm_g.mat", K3D_TOP_DIR "/mcad/colors/gold.mat",
            K3D_TOP_DIR "/mcad/colors/gold.mat" );

    SetupVRML( "hdrtest.wrl", out );
    SetupXForm( "HDR_TEST", out, 0 );
    // square, male
    hdr.SetCase( 6, 1, 2.54, 2.54, 2.72, .72, 0.4 );
    hdr.SetPins( true, true, 2, 10, 0.64, 0.64, 0.64, 0.3, 0.8, 4, 0 );
    hdr.Build( t0, "HDR_MALE_SP", out, 2 );

    hdr.SetColors( K3D_TOP_DIR "/mcad/colors/rbc_crm_g.mat", K3D_TOP_DIR "/mcad/colors/tin.mat",
            K3D_TOP_DIR "/mcad/colors/gold.mat" );
    // square, female
    t0.SetTranslation( 0, -4, 0 );
    hdr.SetCase( 6, 1, 2.54, 2.54, 8, .72, 0.4 );
    hdr.SetPins( true, false, 2, 10, 0.64, 1.6, 0.72, 0.3, 0.8, 24, 0.5 );
    hdr.Build( t0, "HDR_FEMALE_SP", out, 2 );

    // round, female
    t0.SetTranslation( 0, -8, 0 );
    hdr.SetCase( 6, 1, 2.54, 2.54, 8, .72, 0.4 );
    hdr.SetPins( false, false, 2, 10, 0.64, 1.6, 0.96, 0.3, 0.8, 24, 0.1 );
    hdr.Build( t0, "HDR_FEMALE_RP", out, 2 );

    CloseXForm( out, 0 );
    out.close();
    return 0;
}


int testHdrbase()
{
    VRMLMAT color;
    TRANSFORM   t0;
    HDRBASE     h;
    ofstream    out;

    color.Load( "../../../../../mcad/colors/rbc_crm_g.mat" );
    h.SetParams( 2.54, 2.54, 0.4, 2.72, 0.72, 0.9, 1.6, false, 5, 3, 24 );

    SetupVRML( "hdrtest.wrl", out );
    SetupXForm( "TEST_HDR", out, 0 );
    h.Build( t0, color, false, out, 2 );
    // h.Stitch(t0, color, false, out, 2);
    // h.Paint(t0, color, false, out, 2);
    t0.SetTranslation( 0, -10, 5 );
    t0.SetRotation( M_PI * 0.25, 0, 0, 1 );
    h.Build( t0, color, true, out, 2 );
    CloseXForm( out, 0 );
    out.close();
    return 0;
}
