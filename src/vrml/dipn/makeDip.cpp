/**
  file: makeDip.cpp

  Copyright 2012, Cirilo Bernardo

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

  Notes:
    This is a quick and dirty test of the dip class for
    creating DIPn VRML models for use with KiCad.

    TODO:
        + implement batch processing by providing the an input file
            on the command line or optionally streaming an input file
            via stdin.
        + the input file will contain immutable data controlling the
            dimensions of the DIPn package and pins.
        + the input file will contain a comma separated list of the
            number of pins on each package to be generated.
        + The output file name shall be dipN + SUFFIX; the suffix shall
            be a member of the struct 'dipparams'. In the test code
            the suffix is coerced based on the setting of the `metric'
            parameter and the value of the pin row spacing.
 */

#include <stdio.h>

#include "dip.h"
#include "transform.h"

int main()
{
  dip dil;
  char name[256];
  int i;
  dipparams dp;
  dp.casematerial.Load("../../mat/defs/ceram_gry.mat");
  dp.pinmaterial.Load("../../mat/defs/tin.mat");
  dil.SetParams(dp);

  for(i = 4; i <= 28; i+=2) {
    sprintf(name, "dip%d_300i.wrl", i);
    dil.Calc(i, name);
  }

  return 0;
}
