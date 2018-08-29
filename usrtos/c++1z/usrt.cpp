#include<layout.hpp>

#include <boost/python.hpp>
using namespace boost::python;

namespace usrtos {

class _usrtos_mem_obj {
private:
	CPBlock *m_mem;
	Layout::UsrtMem *m_obj;
	bool m_attached = false;
public:
	_usrtos_mem_obj(std::string fn) {
		m_mem = new CPBlock;
		m_attached = m_mem->attach(fn);
		if(m_attached)
			m_obj = new typename Layout::UsrtMem(*m_mem);
	};
	
	void dump() {
		m_obj->dump();
	};

	void dumpHead() {
		m_mem->dumpHead();
	};
};

BOOST_PYTHON_MODULE(usrtos)
{
	class_<_usrtos_mem_obj>("UsrtMem", init<std::string>())
		.def("dump", &_usrtos_mem_obj::dump)
		.def("dumpHead", &_usrtos_mem_obj::dumpHead)
	;
}
};

