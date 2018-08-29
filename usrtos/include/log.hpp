#include "usrtmem.hpp"
#include <map>

namespace usrtos {
template<size_t AT, size_t END>
class log : public Fifo<AT,END> {
public:
	log(CPBlock& m) : Fifo<AT,END>(m) {};
	void printf(int level,const char * format, ...) {
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

class logs {
public:
	typedef log<0x1000,0x10000> usrtlog;

private:
	std::map<std::string,usrtlog*> m_logs;

public:

	logs(std::map<std::string,CPBlock*>& blocks) {
		for(auto it = blocks.begin();it != blocks.end();++it) {
			if(memcmp("log",it->second->m_head->name,3)==0) {
				std::string bn(it->second->m_head->name);
				usrtlog *l = new usrtlog(*(it->second));
				m_logs[bn] = l;
			}
		}
	};

	usrtlog *operator[](std::string l) {
		auto iter = m_logs.find(l);
		if( iter!=m_logs.end() )
			return m_logs[l];
		else
			return nullptr;
	};

	bool has(std::string l) {
		auto iter = m_logs.find(l);
		if( iter!=m_logs.end() )
			return true;
		else
			return false;
	};
};
}; // namesapce usrtos
