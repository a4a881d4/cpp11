#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/sha1.hpp>
#include <unistd.h>


/*
class Head(Structure):
	_fields_ = [ ('name',     c_char*32)   #000
				,('type',     c_char*32)   #032
				,('metaSize', c_longlong)  #064
				,('dataSize', c_longlong)  #072
				,('cpSize',   c_longlong)  #080
				,('longitude',c_double)    #088
				,('latitude', c_double)    #096
				,('altitude', c_double)    #104
				,('version',  c_char*40)   #112
				,('sha1',     c_char*40)   #152
				]                          #192

*/


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

struct Head {
	char name[32];
	char type[32];
	long long metaSize;
	long long dataSize;
	long long cpSize;
	double longitude;
	double latitude;
	double altitude;
	struct sha1str version;
	struct sha1str sha1;
	boost::interprocess::interprocess_mutex g_mutex;
};


using namespace boost;
using namespace boost::uuids;
using namespace boost::uuids::detail;
using namespace boost::interprocess;

class block {
public:

	typedef struct _globe_pointer {
		uuid id;
		long long offset;
		long long objsize;
	} GP;

	struct Head* m_head;
	block(){};
	std::string m_fileName;
	file_mapping m_file;
	mapped_region m_headHandle;
	mapped_region m_dataHandle;
	mapped_region m_cpHandle;
	uuid usrtosNS = lexical_cast<uuid>("8ea09e05-fd67-5949-a9ab-e722a3dae01c");
	uuid m_uuid;
	bool m_attached = false;
	void *m_base;

	void setFileName(std::string fn) { m_fileName = fn; };
	
	void headFromFile() { headFromFile(m_fileName.c_str()); }

	void headFromFile(const char* fn) {
		m_head = new(struct Head);
		std::filebuf fbuf;
        fbuf.open(fn, std::ios_base::in | std::ios_base::binary);
		fbuf.sgetn((char *)(m_head), sizeof(struct Head));
		fbuf.close();
	};

	void dumpHead() {
		const struct Head& s = *m_head;
		auto& [m0,m1,m2,m3,m4,m5,m6,m7,m8,m9,m10] = s;
		std::cout << m0 << std::endl;
		std::cout << m1 << std::endl;
		std::cout << m2 << std::endl;
		std::cout << m3 << std::endl;
		std::cout << m4 << std::endl;
		std::cout << m5 << std::endl;
		std::cout << m6 << std::endl;
		std::cout << m7 << std::endl;
		std::cout << m8 << std::endl;
		std::cout << m9 << std::endl;
	};

	virtual void dump() {
		dumpHead();
		std::cout << m_fileName << std::endl;
	};

	virtual bool checkHead() {
		sha1 sha;
		char *szMsg = (char *)(m_head);
		sha.process_bytes(szMsg, 152);
		unsigned int digest[5];
		sha.get_digest(digest);
		std::stringstream s1,s2;
    	for (int i = 0; i< 5; ++i)
			s1 << std::hex << digest[i];
			
		for(int i = 0;i<40;i++) {
			//std::cout << m_head->sha1.sha1[i] << " ";
			s2 << m_head->sha1.sha1[i];
		}
		if(s1.str()!=s2.str()) {
			//std::cout<<"s1:"<<s1.str().c_str()<<" s2:"<<s2.str()<<std::endl;
			return false; 
		}
		name_generator ngen(usrtosNS);
		m_uuid = ngen(s1.str().c_str());
		std::string stru1 = lexical_cast<std::string>(m_uuid);
		auto fnLen = m_fileName.size();
		if(m_fileName.find(stru1) == std::string::npos) {
			//std::cout << stru1 << " " << m_fileName << std::endl;
			return false; 
		}
		return true;
	};

	void resetMutex(interprocess_mutex& m) {
		auto pm = new(&m) interprocess_mutex;
	};

	bool checkMutex(interprocess_mutex& m) {
		int c = 10;
		while(!m.try_lock()) {
			usleep(10);
			c--;
			if(c==0)
				break;
		}
		if(c!=0) {
			m.unlock();
			return true;
		}
		else{
			resetMutex(m);
			std::cout << "fail to get lock" << std::endl;
			return false;
		}
	};

	bool checkLock() {
		return checkMutex(m_head->g_mutex);
	};

	bool checkCP() {
		if(m_head->cpSize==0) return true;
		void *d = m_dataHandle.get_address();
		void *c = m_cpHandle.get_address();
		auto r = memcmp(d,c,m_head->cpSize);
		if(r==0)
			return true;
		std::cout << *(int *)d << " " << *(int *)c << " " << r <<std::endl; 
		return false;
	};
	
	bool checkUUID(uuid& id) {
		return id==m_uuid;
	};

	bool attach(std::string fn) {
		struct block b;
		b.setFileName(fn);
		b.headFromFile();
		if(!b.checkHead()){
			std::cout << "error 0 " << fn << std::endl;
			return false;
		}
		try {
			m_fileName = fn;
			file_mapping mf(fn.c_str(), read_write);
			mf.swap(m_file);

			std::cout << "pass 0" << std::endl;
			std::cout << std::flush;
			mapped_region mh( m_file
				, read_write
				, 0
				, b.m_head->metaSize
				);
			mh.mapped_region::swap(m_headHandle);
			m_head = (struct Head*)(m_headHandle.get_address());

			if(!checkHead()) {
				std::cout << "error 1" << std::endl;
				return false;
			}
			
			std::cout << "pass 1" << std::endl;
			std::cout << std::flush;
			mapped_region*  pmt = new mapped_region(
				  m_file
				, read_write
				, b.m_head->metaSize
				, b.m_head->dataSize + b.m_head->cpSize
				);
			if(pmt->get_size()!=b.m_head->dataSize + b.m_head->cpSize) {
				std::cout << "error 2 " << pmt->get_size() << std::endl;
				return false;
			}
			m_base = pmt->get_address();
			std::cout << "pass 2" << std::endl;
			std::cout << std::flush;
			
			delete pmt;
			
			mapped_region md( m_file
				, read_write
				, b.m_head->metaSize
				, b.m_head->dataSize
				, m_base
				);
			md.swap(m_dataHandle);
			std::cout << "pass 3" << std::endl;
			std::cout << std::flush;
			
			if( b.m_head->cpSize>0 ){
				mapped_region mc( m_file
					, read_write
					, b.m_head->metaSize
					, b.m_head->cpSize
					, static_cast<char*>(m_base) + (size_t)b.m_head->dataSize
					);
				mc.swap(m_cpHandle);
			}
			std::cout << "pass 4" << std::endl;
			std::cout << std::flush;
			m_attached = checkHead()&&checkCP()&&checkLock();
		}
		catch(interprocess_exception &ex){
			std::cout << "error 3" << std::endl;
			auto l = sizeof(m_head->g_mutex);
			std::cout << ex.what() << " " << l << " " << &(m_head->g_mutex) << " " << m_head << std::endl;
			std::cout << std::hex << *(long long*)(&(m_head->g_mutex));
			std::cout << std::endl;
			resetMutex(m_head->g_mutex);
			std::cout << "affter init" << " ";
			std::cout << std::hex << *(long long*)(&(m_head->g_mutex));
			std::cout << std::endl;
			return false;
		}
		
		return m_attached;
	};

	bool checkType(const char* type) {
		// char b[32];
		// memset(b,0,32);
		// strcpy(b,type);
		auto typeOK = strcmp(m_head->type,type);
		return (typeOK==0);
	};
	
	template <size_t MetaSize>
	bool checkMetaSize() {
		return (m_head->metaSize==MetaSize);
	}
	template<typename Addon, size_t pos>
	const Addon *get_addons() {
		return static_cast<Addon*>((char*)m_head+pos);
	};
	template<typename T>
	T* GP2LP(GP& gp) {
		if(!checkUUID(gp.id))
			return nullptr;
		if(gp.objsize!=(long long)sizeof(T))
			std::cout << "invalid size" << std::endl;
		T* r = static_cast<T*>((char *)m_head+(gp.offset%m_head->dataSize));
		return r;
	}
	~block() {
	};
};

template <typename Addon, typename type_t, size_t pos, size_t MetaSize>
class MemBlock : public block {
public:
	virtual bool checkHead() {
		return block::checkHead() && checkMetaSize<MetaSize>() && checkType(type_t::_type());
		// return checkType(type_t::_type()) 
		// && (block*)this->checkHead() 
		// && checkMetaSize<MetaSize>();
	};
	Addon *get_addons() {
		return (Addon*)((char*)m_head+pos);
	};
};

struct bar {
	size_t pos;
	boost::interprocess::interprocess_mutex bar_mutex; 
};

template <typename Addon, typename type_t, size_t pos, size_t MetaSize>
class MemHeap : public MemBlock<Addon, type_t, pos, MetaSize> {
public:
	virtual void dump() {
		block::dump();
		std::cout << (MemBlock<Addon, type_t, pos, MetaSize> *)this->get_addons()->pos << std::endl;
	};
	template <typename T>
	T* newLP(size_t aligned=1) {
		size_t s = sizeof(T);
		auto pa = (MemBlock<Addon, type_t, pos, MetaSize> *)this->get_addons();
		void *r = nullptr;
		try {
			scoped_lock<interprocess_mutex> lock(pa->bar_mutex);
			if(aligned!=1) {
				size_t m = pa->pos%aligned;
				if(m!=0)
					pa->pos += (aligned-m); 
			}
			r = (void *)((char *)block::m_base+pa->pos);
			pa->pos += s;
			pa->pos %= block::m_head->dataSize;
		}
		catch(interprocess_exception &ex){
			std::cout << "error newLP" << std::endl;
			std::cout << ex.what() << std::endl;
			resetMutex(pa->bar_mutex);
			return nullptr;
		}
		return static_cast<T*>(r);
	}; 
};

class MemHeapType {
	public:
	static const char * _type() { return "MemHeap"; };
};
class BlockType {
	public:
	static const char * _type() { return "block"; };
};
class MemoryBlockLayout {
	
public:
	typedef MemHeap<struct bar, MemHeapType, 0x200, 0x1000> UsrtMem;
	typedef MemHeap<struct bar, BlockType, 0x200, 0x1000> UsrtBlk;
};

