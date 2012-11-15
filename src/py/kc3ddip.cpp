#include <boost/python.hpp>
#include <iostream>

#include "dip.h"

using namespace boost::python;


BOOST_PYTHON_MODULE(kc3ddip)
{
#include "dil.pyinc"
}
