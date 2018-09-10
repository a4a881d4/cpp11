#pragma once

#include <boost/uuid/uuid.hpp>
#include <map>
using namespace std;

namespace usrtos {
struct _globe_pointer {
	boost::uuids::uuid id;
	long long offset;
	long long objsize;
};
struct _memory_region {
	uuid id;
	void *base;
	void *end;
	bool attached;

	bool validAddress(void *p) {
		return (p >= base) && (p < end);
	};

	typedef map<uuid,_memory_region*> MemoryMap;
	
	template<typename T>
	T *GP2LP(MemoryMap& m, _globe_pointer& gp) {
		auto it = m.find(gp.id);
		if(it != m.end()) {
			size_t s = static_cast<char *>(it->second->end) - static_cast<char *>(it->second->base);
			size_t pos = gp.offset%s;
			return static_cast<T*>(static_cast<char *>(it->second->base)+pos);
		}
		else {
			return nullptr;
		}
	};
	
	template<typename T>
	bool LP2GP(MemoryMap& m, _globe_pointer& gp, T* p) {
		for(auto it = m.begin(); it != m.end(); ++it) {
			if(it->second->validAddress(p)) {
				gp.id = it->second->id;
				gp.offset = static_cast<char*>(p)-static_cast<char*>(it->second->base);
				gp.objsize = sizeof(T);
				return true;
			}
		}
		return false;
	};
};
};