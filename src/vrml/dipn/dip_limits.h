/**
  file: dip_limits.h

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
    Extreme values for parameters; these are deliberately very permissive.
    Values are nominally in inches (25.4mm).
*/

#ifndef DIP_LIMITS_H
#define DIP_LIMITS_H

/* case parameters */
/// minimum case height above board
#define MIN_BASE_HEIGHT (0.0)
/// maximum case height above board
#define MAX_BASE_HEIGHT (2.0)

/// minimum case depth
#define MIN_CASE_DEPTH (0.02)
/// maximum case depth
#define MAX_CASE_DEPTH (2.0)

/// minimum depth of the case middle portion (where pins attach)
#define MIN_CASEMID (0.01)
/// maximum depth of the case middle portion
#define MAX_CASEMID (1.0)

/// minimum case length
#define MIN_CASELEN (0.01)
/// maximum case length
#define MAX_CASELEN (10.0)

/// minimum case width
#define MIN_CASEWIDTH (0.02)
/// maximum case width
#define MAX_CASEWIDTH (4.0)

/// minimum case bevel
#define MIN_CASEBEV (0.0002)
/// maximum case bevel
#define MAX_CASEBEV (0.02)

/// minimum case taper
#define MIN_CASETAPER (0.0)
/// maximum case taper
#define MAX_CASETAPER (0.2)

/// minimum notch width
#define MIN_NOTCHWIDTH (0.005)
/// maximum notch width
#define MAX_NOTCHWIDTH (4.0)

/// minimum notch depth
#define MIN_NOTCHDEPTH (0.005)
/// maximum notch depth
#define MAX_NOTCHDEPTH (0.75)

/// minimum notch length
#define MIN_NOTCHLEN (0.005)
/// maximum notch depth
#define MAX_NOTCHLEN (2.0)

/* pin parameters */
/// minimum breadth of a pin (widest part)
#define MIN_PINBREADTH (0.005)
/// maximum breadth of a pin (widest part)
#define MAX_PINBREADTH (0.5)

/// minimum width for the thru portion of a pin
#define MIN_PINWIDTH (0.005)
/// maximum width for the thru portion of a pin
#define MAX_PINWIDTH (0.25)

/// minimum height of the tapered portion
#define MIN_TAPERHEIGHT (0.0)
/// maximum height of the tapered portion
#define MAX_TAPERHEIGHT (0.25)

/// minimum dogear bend on the pin
#define MIN_DOGEAR (0.0)
/// maximum dogear bend on the pin
#define MAX_DOGEAR (0.25)

/// minimum underside offset for the dogear
#define MIN_DOGOFF (0.0)
/// maximum underside offset for the dogear
#define MAX_DOGOFF (0.25)

/// minimum material thickness of the pin
#define MIN_THICK (0.002)
/// maximum material thickness of the pin
#define MAX_THICK (0.1)

/// minimum row spacing for the pins
#define MIN_ROWSPACE (0.02)
/// maximum row spacing for the pins
#define MAX_ROWSPACE (4.0)

/// minimum length of the THRU portion of the pin
#define MIN_THRU (0.04)
/// maximum length of the THRU portion of the pin
#define MAX_THRU (2.0)

/// minimum bevel on a pin edge
#define MIN_PINBEV (0.0001)
/// maximum bevel on a pin edge
#define MAX_PINBEV (0.02)

#endif // DIP_LIMITS_H
