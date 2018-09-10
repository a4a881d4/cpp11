#pragma once
#include <log.hpp>
#include <layout.hpp>

namespace usrtos {
using usrtlog = Layout::UsrtLog;

class logs {
private:
	std::map<std::string,usrtlog*> m_logs;

public:

	logs(std::map<std::string,CPBlock*>& blocks) {
		for(auto it = blocks.begin();it != blocks.end();++it) {
			if(memcmp("log",it->second->getHead()->name,3)==0) {
				std::string bn(it->second->getHead()->name);
				usrtlog *l = new usrtlog(*(it->second));
				l->init();
				m_logs[bn] = l;
			}
		}
	};

	usrtlog *operator[](std::string l) {
		auto iter = m_logs.find(l);
		if(iter != m_logs.end())
			return m_logs[l];
		else {
			std::cout << " cannot find " << l << " log" << std::endl;
			return nullptr;
		}
	};

	bool has(std::string l) {
		auto iter = m_logs.find(l);
		if(iter != m_logs.end()) {
			return true;
		}
		else
			return false;
	};

};
}; // namespace usrtos
