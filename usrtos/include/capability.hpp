#ifndef __capability_H
#define __capability_H
#include <usrtkey.hpp>

namespace usrtos {
struct CapabilityMeta {
	char name[32];
	char type[32];
	struct sha1str version;
	struct sha1str sha1;
};

class CCapability {
public:
	
	CCapability(){};
	~CCapability(){};
	
	
	uuid meta2uuid(const CapabilityMeta& m) {
		sha1 sha;
		UsrtKey::strn2sha1((char *)(&m),sha,32+32+40);
		string s1 = UsrtKey::sha2string(sha);
		sha1Set(m.sha1,s1.c_str());
		uuid id = UsrtKey::str2key(s1);
		return id;
	};

	virtual uuid getKey() { 
		return UsrtKey::usrtosNS(); 
	};
	
	virtual int run( void *argv ) { return -1; };
	virtual int destroy() { return -1; };

};
}; //namespace usrtos
#endif		