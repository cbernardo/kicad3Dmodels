#include <boost/python.hpp>

#include <fstream>

#include "material.h"
#include "vrmlmat.h"
#include "transform.h"
#include "vcom.h"
#include "polygon.h"
#include "rectangle.h"
#include "circle.h"
#include "shoulder.h"
#include "hole.h"
#include "funnel.h"
#include "pin.h"


using namespace boost::python;
using namespace kc3d;

/*
 *       OVERLOADS
 */
// std::ofstream
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ofs_open_var, open, 1, 2)
bool (std::ofstream::*ofs_isopen)() = &std::ofstream::is_open;

// common
#include "transform.pydef"

// vrml/common
#include "polygon.pydef"


BOOST_PYTHON_MODULE(kc3d)
{
  object class_ofstream = class_<std::ofstream, boost::noncopyable>("ofstream")
    .def("open", &std::ofstream::open, ofs_open_var())
    .def("close", &std::ofstream::close)
    .def("good", &std::ofstream::good)
    .def("is_open", ofs_isopen)
  ;
  
  object instance_ofstream = class_ofstream();

#include "material.pyinc"
#include "vrmlmat.pyinc"
#include "transform.pyinc"
#include "vcom.pyinc"
#include "polygon.pyinc"
#include "rectangle.pyinc"
#include "circle.pyinc"
#include "shoulder.pyinc"
#include "hole.pyinc"
#include "funnel.pyinc"
#include "pin.pyinc"
}

