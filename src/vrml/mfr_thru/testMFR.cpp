/*
 * Quick and dirty test program for thru-hole resistors
 */

#include <iostream>
#include <fstream>
#include <string>

#include "polygon.h"
#include "vcom.h"
#include "transform.h"
#include "vrmlmat.h"
#include "resistor.h"

using namespace std;

int main()
{
    RParams rp;
    // load the colors for 0..9
    rp.colors[0].Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_blk_g.mat");
    rp.colors[1].Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_brn_g.mat");
    rp.colors[2].Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_red_g.mat");
    rp.colors[3].Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_org_g.mat");
    rp.colors[4].Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_yel_g.mat");
    rp.colors[5].Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_grn_g.mat");
    rp.colors[6].Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_blu_g.mat");
    rp.colors[7].Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_pur_g.mat");
    rp.colors[8].Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_gry_g.mat");
    rp.colors[9].Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_wht_g.mat");
    // load the gold and silver tolerance colors
    rp.colors[10].Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_gld_g.mat");
    rp.colors[11].Load("/home/cirilo/projects/kicad/models/mcad/colors/rcc_slv_g.mat");
    // load the body color
    rp.colors[12].Load("/home/cirilo/projects/kicad/models/mcad/colors/rbc_blu_g.mat");
    // load the wire color
    rp.colors[13].Load("/home/cirilo/projects/kicad/models/mcad/colors/tin.mat");

    rp.shift = -rp.p/2.0;
    // rp.d = 0.55;  // L1
    // rp.D = 2.3;   // L1
    // rp.L = 6.2;   // L1
    rp.d = 0.45;  // L2024
    rp.D = 2;   // L2024
    rp.L = 3.7;   // L2024
    rp.wsides = 16;
    rp.bsides = 6;
    rp.rsides = 48;
    rp.endshape = 'B';
    rp.bcap = true; // place end caps
    char code[] = "44X88X99X11XXXSSSX44";
    Resistor res;
    // int Create(RParams &rp, char bands[6], std::ofstream &fp, const std::string &filename);
    res.Create(rp, code, "testres");
    
    return 0;
}
