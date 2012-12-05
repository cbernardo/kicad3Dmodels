#include <boost/python.hpp>
#include <iostream>

#include "do35.h"
#include "gendiode.h"

using namespace boost::python;

#include "diode.pydef"

BOOST_PYTHON_MODULE(kc3ddiode)
{
#include "diode.pyinc"
}
