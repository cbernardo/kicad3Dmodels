#include <boost/python.hpp>
#include <iostream>

#include "resistor.h"

using namespace boost::python;

#include "resistor.pydef"

BOOST_PYTHON_MODULE(kc3dres)
{
#include "resistor.pyinc"
}
