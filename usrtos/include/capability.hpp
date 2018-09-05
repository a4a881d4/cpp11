#ifndef __capability_H
#define __capability_H
#include <captype.hpp>
#include <usrtkey.hpp>

#include <iomanip>
#include <sstream>

using namespace boost;
using namespace boost::uuids;
using namespace boost::uuids::detail;

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
		char *szMsg = (char *)(&m);
		
		sha.process_bytes(szMsg, 32+32+40);
		unsigned int digest[5];
		sha.get_digest(digest);
		std::stringstream s1;
	
    	for (int i = 0; i< 5; ++i)
			s1 << std::setfill('0') << std::setw(8) 
			   << std::hex << digest[i];

		// for(int i = 0;i<40;i++) {
		// 	m.sha1.sha1[i] = s1.str()[i];
		// }
		sha1Set(m.sha1,s1.str().c_str());

		name_generator ngen(CCapability::usrtosNS());
		uuid id = ngen(s1.str().c_str());
		return id;
	};

	virtual uuid getKey() { 
		return CCapability::usrtosNS(); 
	};
	
	virtual int run( void *argv ) { return -1; };
	virtual int destroy() { return -1; };

};
#endif		