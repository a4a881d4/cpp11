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
    	vsnprintf (buffer,1023,format, args);
    	va_end (args);
    	buffer[1023]='\0';
    	int lenLog = strlen(buffer)+1;
    	char *gb = Mem<AT>::template newLP<char>(lenLog);
    	memcpy(gb,buffer,lenLog);
    	Fifo<AT,END>::template push<char>(gb);
	};
	void init() { Fifo<AT,END>::local_reset(); };
	char *getLog() { return Fifo<AT,END>::template local_get<char>(); };
	void dump() {
		int l = Fifo<AT,END>::local_len();
		for(int i=0;i<l;i++)
			std::cout << getLog() <<std::endl;
	};
};
}; // namesapce usrtos
