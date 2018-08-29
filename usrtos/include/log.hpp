#include "usrtmem.hpp"

template<size_t AT, size_t END>
class log : public Fifo {
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
    	char *gb = newLP<char>(lenLog);
    	memcpy(gb,buffer,lenLog);
    	push<char>(gb);
	};
	void init() { local_reset(); };
	char *getLog() { return local_get<char>(); };
	void dump() {
		int l = local_len();
		for(int i=0;i<l;i++)
			std::cout << m_mem->m_head->name << ": " << getLog() <<std::endl;
	};
};

class logs {
public:
	typedef log<0x1000,0x10000> usrtlog;

private:
	map<std::string,usrtlog*> m_logs;

public:

	logs(map<std::string,CPBlock*>& blocks) {
		for(auto it = blocks.begin();it != blocks.end();++it) {
			if(memcmp("log",it->second->m_head->name,3)==0) {
				std::string bn(it->second->m_head->name);
				usrtlog *l = new usrtlog(*(it->second));
				m_logs[bn] = l;
			}
		}
	};

	usrtlog *operator[](std::string l) {
		auto iter = m_logs.find(l)
		if( iter!=m_logs.end() )
			return m_logs[l];
		else
			return nullptr;
	};

	bool has(std::string l) {
		auto iter = m_logs.find(l)
		if( iter!=m_logs.end() )
			return true;
		else
			return false;
	};
};