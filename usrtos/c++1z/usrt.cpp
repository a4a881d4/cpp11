#include "pusrtmem.hpp"
#include "pusrtconfig.hpp"

#include <boost/python.hpp>
using namespace boost::python;

namespace usrtos {

BOOST_PYTHON_MODULE(usrtos)
{
	class_<_usrtos_mem_obj>("UsrtMem", init<std::string>())
		.def("dump", &_usrtos_mem_obj::dump)
		.def("dumpHead", &_usrtos_mem_obj::dumpHead)
	;

	class_<CPBlock::GP>("gp")
	;

	class_<UsrtConfig>("UsrtConfig", init<std::string>())
		.def("HelloWorld", &UsrtConfig::HelloWorld)
		.def("byKeyInt", &UsrtConfig::byKeyInt)
		.def("byKeyStr", &UsrtConfig::byKeyStr)
		.def("byKeyKey", &UsrtConfig::byKeyKey)
		.def("byKey", &UsrtConfig::byKey)
		.def("getKey", &UsrtConfig::getKey)
	;

}
};

