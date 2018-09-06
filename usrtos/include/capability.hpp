#ifndef __capability_H
#define __capability_H
#include <usrtkey.hpp>

namespace usrtos {


class CCapability {
public:
	
	CCapability(){};
	~CCapability(){};
	
	virtual uuid getKey() { 
		return UsrtKey::usrtosNS(); 
	};
	
	virtual int run( void *argv ) { return -1; };
	virtual int destroy() { return -1; };

};
}; //namespace usrtos
#endif		