
#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/sha1.hpp>
#include <iomanip>
#include <sstream>
#include <string.h>
#include <iostream>

using namespace boost;
using namespace boost::uuids;
using namespace boost::uuids::detail;

using namespace std;

namespace usrtos {

struct sha1str {
	char sha1[40];
	sha1str(){};
	sha1str(sha1str& a) {
		memcpy(&sha1[0],&(a.sha1),40);
	};
	sha1str(const char s[]) {
		strncpy(&sha1[0],s,40);
	};
};

inline ostream& operator<<(ostream& os,const struct sha1str& h)
{
	char buf[50];
	memcpy(buf,&(h.sha1),40);
	buf[40]='\0';
	os << buf;
	return os;
}
inline void sha1Set(const struct sha1str& h, const char s[]) {
	char buf[50];
	strncpy(buf,s,40);
	memcpy((void *)&(h.sha1),buf,40);
};

string sha2str(const struct sha1str& s) {
	char buf[50];
	strncpy(buf,(const char *)&s,40);
	buf[40]='\0';
	return string(buf);
};

struct UsrtKey {
	static const uuid usrtosNS() { 
		return lexical_cast<uuid>("8ea09e05-fd67-5949-a9ab-e722a3dae01c"); 
	};
	static const uuid keySentinel() { 
		return lexical_cast<uuid>("8ea09e05-fd67-5949-a9ab-e722a3dae01c"); 
	};
	static string sha2string(sha1& sha) {
		unsigned int digest[5];
		sha.get_digest(digest);
		stringstream s1;
    	for (int i = 0; i< 5; ++i)
			s1 << setfill('0') << setw(8) << hex << digest[i];
		string r = s1.str();
		return r;
	};

	static void strn2sha1(char *msg, sha1& sha, size_t len) {
		sha.process_bytes(msg, len);
	};

	static uuid str2key(string& s1) {
		name_generator ngen(usrtosNS());
		uuid id = ngen(s1.c_str());
		return id;
	};

	static string key2string(uuid& id) {
		string stru1 = lexical_cast<string>(id);
		return stru1;
	};
};
}; //namesapce usrtos