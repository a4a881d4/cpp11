#pragma once

#include <usrtworker.hpp>

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
};