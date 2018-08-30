#pragma once
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
#include <iomanip>

namespace usrtos {

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

class CPBlock {
public:

	typedef struct _globe_pointer {
		uuid id;
		long long offset;
		long long objsize;
		
	} GP;

	struct Head* m_head;
	CPBlock(){};
	std::string m_fileName;
	file_mapping m_file;
	mapped_region m_headHandle;
	mapped_region m_dataHandle;
	mapped_region m_cpHandle;
	uuid m_uuid;
	bool m_attached = false;
	void *m_base;
	void *m_end;

	static const uuid usrtosNS() { return lexical_cast<uuid>("8ea09e05-fd67-5949-a9ab-e722a3dae01c"); };
	
	void setFileName(std::string fn) { m_fileName = fn; };
	
	static struct Head& sheadFromFile(const char* fn) {
		auto p_head = new(struct Head);
		std::filebuf fbuf;
        fbuf.open(fn, std::ios_base::in | std::ios_base::binary);
		fbuf.sgetn((char *)(p_head), sizeof(struct Head));
		fbuf.close();
		return *p_head;
	}
	void headFromFile() { headFromFile(m_fileName.c_str()); }
	void headFromFile(const char* fn) {
		m_head = &CPBlock::sheadFromFile(fn);
	};

	void dumpHead() {
		const struct Head& s = *m_head;
		CPBlock::sdumpHead(s);
	}
	
	static void sdumpHead(const struct Head& s) {
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
		const struct Head& s = *m_head;
		return CPBlock::scheckHead(m_fileName, s, &m_uuid);
	};

	static bool scheckHead(std::string fn, const struct Head& s, uuid *pid = nullptr) {
		sha1 sha;
		
		char *szMsg = (char *)(&s);
		
		sha.process_bytes(szMsg, 152);
		unsigned int digest[5];
		sha.get_digest(digest);
		
		std::stringstream s1,s2;
    	
    	for (int i = 0; i< 5; ++i)
			s1 << std::setfill('0') << std::setw(8) << std::hex << digest[i];
			
		for(int i = 0;i<40;i++) {
			s2 << s.sha1.sha1[i];
		}
		
		if(s1.str()!=s2.str()) {
			std::cout<<"s1:"<<s1.str()<<" s2:"<<s2.str()<<std::endl;
			return false; 
		}
		
		name_generator ngen(CPBlock::usrtosNS());
		uuid id = ngen(s1.str().c_str());
		
		std::string stru1 = lexical_cast<std::string>(id);
		
		auto fnLen = fn.size();
		
		if(fn.find(stru1) == std::string::npos) {
			std::cout << stru1 << " : " << fn << std::endl;
			return false; 
		}
		if(pid!=nullptr)
			*pid = id;
		return true;
	};

	void resetMutex(interprocess_mutex& m) {
		auto pm = new(&m) interprocess_mutex;
	};

	bool checkMutex(interprocess_mutex& m, std::string mn) {
		int c = 10;
		try {
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
				std::cout << "fail to get lock: " << mn << std::endl;
				return false;
			}
		}
		catch(interprocess_exception &ex){
			std::cout 
				<< ex.what() 
				<< " name: " << m_head->name 
				<< " type: " << m_head->type 
				<< " mutex " << mn
				<< std::endl;
			resetMutex(m);
			return false;
		}
	};

	virtual bool checkLock() {
		return checkMutex(m_head->g_mutex, "g_mutex");
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
		struct CPBlock b;
		b.setFileName(fn);
		b.headFromFile();
		if(!b.checkHead()) {
			std::cout << "error 0 " << fn << std::endl;
			return false;
		}

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
		m_end = (void *)((char *)m_base+m_head->dataSize);
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
		
		return m_attached;
	};

	bool checkType(const char* type) {
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
		T* r = Off2LP<T>(gp.offset);
		return r;
	}

	template <typename T>
	T* Off2LP(size_t offset) {
		return static_cast<T*>((void *)((char *)m_base+(offset%m_head->dataSize)));
	};

	template <typename T>
	size_t LP2offset(T* p) {
		void *vp = static_cast<void *>(p);
		if(vp>=m_base && vp<m_end) {
			return (char *)vp-(char *)m_base;
		}
		else
			return m_head->dataSize;
	};

	template <typename T>
	bool LP2GP(GP& gp, T* p) {
		auto off = LP2offset<T>(p);
		if(off==m_head->dataSize)
			return false;
		gp.id = m_uuid;
		gp.offset = off;
		gp.objsize = sizeof(T);
		return true;
	};

	bool validOffset(size_t off){
		return off<m_head->dataSize;
	};
	
	~CPBlock() {
	};
};
}; // namesapce usrtos 