#pragma once
#include <ostream>
#include <string.h>

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

inline std::ostream& operator<<(std::ostream& os,const struct sha1str& h)
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

std::string sha2str(sha1str& s) {
	char buf[50];
	strncpy(buf,s,40);
	return std::string(buf);
};