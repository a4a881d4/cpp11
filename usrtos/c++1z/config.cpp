#include "usrtconfig.hpp"

#include <boost/python.hpp>
using namespace boost::python;

namespace usrtos {

BOOST_PYTHON_MODULE(usrtos)
{
	class_<UsrtConfig>("UsrtConfig", init<std::string>())
		.def("HelloWorld", &UsrtConfig::HelloWorld)
		.def("byKeyInt", &UsrtConfig::byKeyInt)
		.def("byKey", &UsrtConfig::byKey)
		.def("getKey", &UsrtConfig::getKey)
	;

}
};

