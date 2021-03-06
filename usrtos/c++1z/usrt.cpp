#include <version.hpp>

#include "pusrtmem.hpp"
#include "pusrtconfig.hpp"
#include "pusrttask.hpp"
#include "pusrtscript.hpp"
#include "pusrtclock.hpp"
#include <glog.hpp>

#include <boost/python.hpp>

#include <internalfuncmap.hpp>

using namespace boost::python;
namespace usrtos {
using namespace vm;

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

	class_<ANYTYPE>("AnyType", init<size_t>())
		.def("pack", &ANYTYPE::pack)
		.def("setUUID", &ANYTYPE::setUUID)
		.def("setString", &ANYTYPE::setString)
		;

	class_<UsrtClock>("uClock")
		.def("peek",&UsrtClock::peek)
		.def("cupdate",&UsrtClock::update)
		.def_readonly("cpu",&UsrtClock::cpu)
		.def_readonly("sys",&UsrtClock::sys)
		.def_readonly("system_clock",&UsrtClock::system_clock)
		.def_readonly("wall_clock",&UsrtClock::wall_clock)
		;

	class_<UUID>("UUID", init<std::string>());

	def("validInternalFunc",validInternalFunc);
	def("name2internalFunc",name2internalFunc);
	def("internalFuncName",internalFuncName);
	
	class_<UsrtTaskAPI>("UsrtTaskAPI", init<std::string>())
		.def("HelloWorld", &UsrtTaskAPI::HelloWorld)
		.def("byKeyInt", &UsrtTaskAPI::byKeyInt)
		.def("byKeyStr", &UsrtTaskAPI::byKeyStr)
		.def("getKey", &UsrtTaskAPI::getKey)
		.def("newTask", &UsrtTaskAPI::newTask)
		.def("emitTask", &UsrtTaskAPI::emitTask)
		.def("setCallBack", &UsrtTaskAPI::setCallBack)
		.def("allocMulti", &UsrtTaskAPI::allocMulti)
		.def("setMulti", &UsrtTaskAPI::setMulti)
		.def("setIntArgv", &UsrtTaskAPI::setIntArgv)
		.def("allocDepend", &UsrtTaskAPI::allocDepend)
		.def("dumpTask", &UsrtTaskAPI::dumpTask)
	;

#define ENUM_TEST_OPERATORS(visitOp) \
	visitOp(0x84,moveal,"move_all_fromAtoB",BINARY(U8,U8),std::tuple<U8,U8>) 

	#define VISIT_OPCODE(code,name,...) .def(#name,&UsrtScript::p_##name)
	class_<UsrtScript>("UsrtScript",init<std::string,size_t>())
		.def("push",&UsrtScript::push)
		.def("reset",&UsrtScript::reset)
		.def("test",&UsrtScript::test)
		.def("nTask",&UsrtScript::newTask)
		.def("nScript",&UsrtScript::newScript)
	ENUM_OPERATORS(VISIT_OPCODE) 
	;
	#undef VISIT_OPCODE
}
};

