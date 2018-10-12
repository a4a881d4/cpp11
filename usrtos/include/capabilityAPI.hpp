#pragma once
#ifndef SYSTEM_FUNCTION 
#ifdef FUNCLASS
#include <capability.hpp>
#include <version.hpp>
#include <userhelper.hpp>

#define _STR(x) #x
#define STR(x) _STR(x)
using namespace usrtos;

class FUNCLASS : public CCapability {
private:
	uuid key;
public:
	struct CapabilityMeta meta;
	int run(void *argv);
	uuid getKey() {
		return key;
	};
	FUNCLASS() {
		memset(&meta,0,sizeof(struct CapabilityMeta));
		strncpy((char *)meta.name,STR(FUNCLASS),32);
		strncpy((char *)meta.type,"USRTOS",32);
		meta.version = sha1str(USRTOSVERSION);
		key = UserHelper::meta2uuid(meta);
	};	
	int destroy() {
		delete this;
		return 1;
	};
};

extern "C" {
	CCapability *factroy() {
		FUNCLASS *item = new FUNCLASS();
		return (CCapability*)item;
	}
	int run(CCapability* item, void *argv) {
		return ((FUNCLASS *)item)->run(argv);
	}
	uuid getKey(CCapability* item){
		return ((FUNCLASS *)item)->getKey();
	}
	void destroy(CCapability* item) {
		((FUNCLASS *)item)->destroy();
	}
};	

#else
#error "Must define FUNCLASS Macro"
#endif
#else
#include "system.hpp"
#endif // #ifndef SYSTEM_FUNCTION