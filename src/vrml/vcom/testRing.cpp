/*
 * Test program for the VRML Ring entity
 * 
 * The ring is an ellipse which can render an elliptical surface
 * visible or use vertices from a second ellipse to render a tube.
 * 
 */

#include <iostream>
#include <fstream>
#include <string>

#include "polygon.h"
#include "vcom.h"
#include "transform.h"
#include "vrmlmat.h"

using namespace std;

int main()
{
    ofstream output;
    Transform t0, t1, t2;
    VRMLMat body;
    VRMLMat band;
    Polygon ring[26];

    /*
     *  This aims to test the appearance of colors for resistor bodies
     *  and value codes. The ends shall be comprised of the body color
     *  and the stripes shall be comprised of the values 0..9, Gold, Silver
     *  for a total of 25 bands.  The end bands shall be 1.0 units wide while
     *  the middle bands shall be 0.5 units wide.
     */

    body.Load("/home/cirilo/projects/kicad/models/mcad/colors/rbc_blu_g.mat");
    Rotation rota(M_PI/2.0, 0, 1, 0);
    //Rotation rota(0, 0, 0, 0);
    t0.setRotation(rota);

    Quat offset(0, 1, 0, 0);
    Translation tr(offset);
    t1.setTranslation(tr);
    t1.setRotation(rota);

    ring[0].Calc(48, 0.7, 0.7, t0);
    int i;
    for (i = 1; i < 25; ++i)
    {
        ring[i].Calc(48, 0.7, 0.7, t1);
        tr = tr + Quat(0, 0.5, 0, 0);
        t1.setTranslation(tr);
    }
    tr = tr + Quat(0, 0.5, 0, 0);
    t1.setTranslation(tr);
    ring[i].Calc(48, 0.7, 0.7, t1);
    
    SetupVRML("vrmlRing.wrl", output);
    SetupXForm("TestDisc", output, 0);
    // Create the body
    ring[0].Paint(false, t2, body, false, output, 2);
    for (i = 0; i < 25; i+=2)
    {
        ring[i].Stitch(ring[i+1], true, t2, body, true, output, 2);
    }
    ring[25].Paint(true, t2, body, true, output, 2);
    // Insert the bands
    band.Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_blk_g.mat");
    i = 1;
    ring[i].Stitch(ring[i+1], true, t2, band, false, output, 2);
    band.Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_brn_g.mat");
    i += 2;
    ring[i].Stitch(ring[i+1], true, t2, band, false, output, 2);
    band.Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_red_g.mat");
    i += 2;
    ring[i].Stitch(ring[i+1], true, t2, band, false, output, 2);
    band.Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_org_g.mat");
    i += 2;
    ring[i].Stitch(ring[i+1], true, t2, band, false, output, 2);
    band.Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_yel_g.mat");
    i += 2;
    ring[i].Stitch(ring[i+1], true, t2, band, false, output, 2);
    band.Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_grn_g.mat");
    i += 2;
    ring[i].Stitch(ring[i+1], true, t2, band, false, output, 2);
    band.Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_blu_g.mat");
    i += 2;
    ring[i].Stitch(ring[i+1], true, t2, band, false, output, 2);
    band.Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_pur_g.mat");
    i += 2;
    ring[i].Stitch(ring[i+1], true, t2, band, false, output, 2);
    band.Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_gry_g.mat");
    i += 2;
    ring[i].Stitch(ring[i+1], true, t2, band, false, output, 2);
    band.Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_wht_g.mat");
    i += 2;
    ring[i].Stitch(ring[i+1], true, t2, band, false, output, 2);
    band.Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_gld_g.mat");
    i += 2;
    ring[i].Stitch(ring[i+1], true, t2, band, false, output, 2);
    band.Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_slv_g.mat");
    i += 2;
    ring[i].Stitch(ring[i+1], true, t2, band, false, output, 2);

    CloseXForm(output, 0);
    output.close();
    
    return 0;
}
