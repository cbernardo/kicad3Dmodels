#include <boost/python.hpp>

#include <fstream>

#include "material.h"
#include "vrmlmat.h"
#include "transform.h"



using namespace boost::python;

/*
 *       OVERLOADS
 */
// std::ofstream
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ofs_open_var, open, 1, 2)

// common
#include "transform.pydef"
// vrml/common


BOOST_PYTHON_MODULE(kc3d)
{
  object class_ofstream = class_<std::ofstream, boost::noncopyable>("ofstream")
    .def("open", &std::ofstream::open, ofs_open_var())
    .def("close", &std::ofstream::close)
    .def("good", &std::ofstream::good)
    // XXX - can we expose is_open() ?
  ;
  
  object instance_ofstream = class_ofstream();

#include "material.pyinc"
#include "vrmlmat.pyinc"
#include "transform.pyinc"
}

