#pragma once
#include <usrtmem.hpp>
#include <map>

namespace usrtos {
template<size_t AT, size_t END>
class log : public Fifo<AT,END> {
public:
	struct logitem {
		size_t s;
		char buf[256-8];
	};
	log(CPBlock& m) : Fifo<AT,END>(m) {};
	void printf(const char * format, ...) {
	    logitem *gb = Mem<AT>::template newLP<logitem>(1);
	    va_list args;
    	va_start (args, format);
    	int r = vsnprintf (gb->buf,240,format, args);
    	va_end (args);
    	if(r<0) {
    		std::cout << "vsnprintf error" << std::endl;
    	} else {
	    	int lenLog = strnlen(gb->buf,240);
	    	if(r!=lenLog) {
	    		std::cout << "length of message error" << std::endl;
	    	}
	    	gb->buf[lenLog]='\0';
	    	gb->s = lenLog;
	    	Fifo<AT,END>::template push<logitem>(gb);
    	}
	};
	void put(std::string& s) {
	    logitem *gb = Mem<AT>::template newLP<logitem>();
	    int size = s.size();
	    if(size > 239)
	    	size = 239;
	    	
	    strncpy(gb->buf,s.c_str(),size);
	    gb->buf[239] = '\0';
	   	gb->s = s.size();
	    Fifo<AT,END>::template push<logitem>(gb);
    };
	void init() { Fifo<AT,END>::local_reset(); };
	logitem *getLog() { return Fifo<AT,END>::template local_get<logitem>(); };
	void dump() {
		int l = Fifo<AT,END>::local_len();
		for(int i=0;i<l;i++) {
			const logitem * str = getLog();
			char cstr[256];
			char *s = (char *)str->buf,*o = cstr;
			int c = str->s;
			if(c > 240) {
				cout << "too long " << c << endl;
				c = 239;
			}
			while(*s != '\0' && c > 0) {
				*o++ = *s++;
				c--;
			}
			*o = '\0';
			cstr[239]='\0';
			std::cout << l << "(" << str->s << "):" << cstr << std::endl; 
		}
	};
};
}; // namesapce usrtos
