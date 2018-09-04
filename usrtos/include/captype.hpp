#pragma once
#include <ostream>
#include <string.h>

struct sha1str {
	char sha1[40];
};

inline std::ostream& operator<<(std::ostream& os,const struct sha1str& h)
{
	char buf[50];
	memcpy(buf,&(h.sha1),40);
	buf[40]='\0';
	os << buf;
	return os;
}