#pragma once
#include <usrtmem.hpp>
#include <map>

namespace usrtos {
template<size_t AT, size_t END>
class log : public Fifo<AT,END> {
public:
	log(CPBlock& m) : Fifo<AT,END>(m) {};
	void printf(const char * format, ...) {
	    char buffer[1024];
    	va_list args;
    	va_start (args, format);
    	int r = vsnprintf (buffer,1023,format, args);
    	va_end (args);
    	if(r<0) {
    		std::cout << "vsnprintf error" << std::endl;
    	} else {
	    	int lenLog = strnlen(buffer,1023);
	    	if(r!=lenLog) {
	    		std::cout << "length of message error" << std::endl;
	    	}
	    	buffer[lenLog]='\0';
	    	char *gb = Mem<AT>::template newLP<char>(lenLog+1);
	    	memcpy(gb,buffer,lenLog+1);
	    	Fifo<AT,END>::template push<char>(gb);
    	}
	};
	void init() { Fifo<AT,END>::local_reset(); };
	char *getLog() { return Fifo<AT,END>::template local_get<char>(); };
	void dump() {
		int l = Fifo<AT,END>::local_len();
		for(int i=0;i<l;i++) {
			const char * str = getLog();
			int len = strnlen(str,256);
			if( len<255 ) {
				std::cout << str <<std::endl;		
			} else {
				std::cout << "message(" << len << ") to long" << std::endl;
				char cstr[256];
				memcpy(cstr,str,255);
				cstr[255]='\0';
				std::cout << cstr << std::endl; 
			}
		}
	};
};
}; // namesapce usrtos
