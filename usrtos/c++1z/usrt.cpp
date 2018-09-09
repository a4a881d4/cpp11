#include <version.hpp>

#include "pusrtmem.hpp"
#include "pusrtconfig.hpp"
#include "pusrttask.hpp"

#include <boost/python.hpp>
using namespace boost::python;

namespace usrtos {

BOOST_PYTHON_MODULE(usrtos)
{
	class_<_usrtos_mem_obj>("UsrtMem", init<std::string>())
		.def("dump", &_usrtos_mem_obj::dump)
		.def("dumpHead", &_usrtos_mem_obj::dumpHead)
		.def("newGP", &_usrtos_mem_obj::newGP)
		.def("GPandLP", &_usrtos_mem_obj::GPandLP)
	;

	class_<CPBlock::GP>("gp")
		.def_readonly("id", &CPBlock::GP::id)
		.def_readonly("offset", &CPBlock::GP::offset)
		.def_readonly("objsize", &CPBlock::GP::objsize)
	;

	class_<UsrtConfig>("UsrtConfig", init<std::string>())
		.def("HelloWorld", &UsrtConfig::HelloWorld)
		.def("byKeyInt", &UsrtConfig::byKeyInt)
		.def("byKeyStr", &UsrtConfig::byKeyStr)
		.def("byKeyKey", &UsrtConfig::byKeyKey)
		.def("byKey", &UsrtConfig::byKey)
		.def("getKey", &UsrtConfig::getKey)
		.def("getWorkerKey", &UsrtConfig::getWorkerKey)
	;

	class_<UsrtTaskAPI>("UsrtTaskAPI", init<std::string>())
		.def("HelloWorld", &UsrtTaskAPI::HelloWorld)
		.def("byKeyInt", &UsrtTaskAPI::byKeyInt)
		.def("byKeyStr", &UsrtTaskAPI::byKeyStr)
		.def("getKey", &UsrtTaskAPI::getKey)
	;
}
};

