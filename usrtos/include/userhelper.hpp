#pragma once

#include <usrtkey.hpp>
namespace usrtos {

struct CapabilityMeta {
	char name[32];
	char type[32];
	struct sha1str version;
	struct sha1str sha1;
};

class UserHelper {
public:
	static uuid meta2uuid(const CapabilityMeta& m) {
		sha1 sha;
		UsrtKey::strn2sha1((char *)(&m),sha,32+32+40);
		string s1 = UsrtKey::sha2string(sha);
		sha1Set(m.sha1,s1.c_str());
		uuid id = UsrtKey::str2key(s1);
		return id;
	};

	static uuid cap2key(std::string capname) {
		struct CapabilityMeta keeper_meta;
		memset(&keeper_meta,0,sizeof(struct CapabilityMeta));
		strncpy((char *)keeper_meta.name,capname.c_str(),31);
		strncpy((char *)keeper_meta.type,"USRTOS",32);
		keeper_meta.version = sha1str(USRTOSVERSION);
		uuid key = UserHelper::meta2uuid(keeper_meta);
		return key;
	};

	static string dumpMem(void *p, size_t len) {
		stringstream s;
		char *cp = (char *)p;
		for(int i = 0;i < len;i++) {
			int a = static_cast<int>(*cp);
			a &= 0xff;
			s << setfill('0') << setw(2) << hex << a << " ";
			if((i&7) == 7)
				s << " ";
			if((i&15) == 15)
				s << endl;
			cp++;
		}
		if((len&16) != 0)
			s << endl;
		return s.str();
	};
};
};