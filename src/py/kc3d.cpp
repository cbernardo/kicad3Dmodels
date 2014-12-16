/*
 *      file: kc3d.cpp
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

#include <boost/python.hpp>

#include <fstream>

#include <material.h>
#include <vrmlmat.h>
#include <transform.h>
#include <vcom.h>
#include <polygon.h>
#include <rectangle.h>
#include <circle.h>
#include <hdrbase.h>
#include <shoulder.h>
#include <hole.h>
#include <funnel.h>
#include <pin.h>
#include <wire.h>
#include <kc3dtess.h>
#include <dimple.h>

using namespace boost::python;
using namespace KC3D;

/*
 *       OVERLOADS
 */
// std::ofstream
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( ofs_open_var, open, 1, 2 )
bool (std::ofstream::* ofs_isopen)() = &std::ofstream::is_open;

// common
#include <transform.pydef>

// vrml/common
#include <polygon.pydef>
#include <wire.pydef>
#include <kc3dtess.pydef>


BOOST_PYTHON_MODULE( kc3d ) {
    object class_ofstream = class_<std::ofstream, boost::noncopyable>( "ofstream" )
                            .def( "open", &std::ofstream::open, ofs_open_var() )
                            .def( "close", &std::ofstream::close )
                            .def( "good", &std::ofstream::good )
                            .def( "is_open", ofs_isopen )
    ;

    object instance_ofstream = class_ofstream();

#include <material.pyinc>
#include <vrmlmat.pyinc>
#include <transform.pyinc>
#include <vcom.pyinc>
#include <polygon.pyinc>
#include <rectangle.pyinc>
#include <circle.pyinc>
#include <hdrbase.pyinc>
#include <shoulder.pyinc>
#include <hole.pyinc>
#include <funnel.pyinc>
#include <pin.pyinc>
#include <wire.pyinc>
#include <kc3dtess.pyinc>
#include <dimple.pyinc>
}
