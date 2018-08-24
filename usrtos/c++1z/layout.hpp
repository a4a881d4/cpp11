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

using namespace boost;
using namespace boost::uuids;
using namespace boost::uuids::detail;
using namespace std;
using namespace boost::interprocess;

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
				,('rdlock',   c_int*10)    #192
				,('wrlock',   c_int*10)    #232
				]                          #272

*/

struct sha1str {
	char sha1[40];
};

ostream& operator<<(ostream& os,const struct sha1str& h)
{
	char buf[50];
	memcpy(buf,&(h.sha1),40);
	buf[40]='\0';
	os<<buf;
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
	interprocess_mutex rd;
	interprocess_mutex wr;
};

struct block {
	struct Head* m_head;
	block(){};
	string m_fileName;
	file_mapping m_file;
	mapped_region m_headHandle;
	mapped_region m_dataHandle;
	mapped_region m_cpHandle;
	uuid usrtosNS = lexical_cast<uuid>("8ea09e05-fd67-5949-a9ab-e722a3dae01c");
	bool m_attached = false;
	void *m_base;

	void setFileName(string fn) { m_fileName = fn; };
	
	void headFromFile() { headFromFile(m_fileName.c_str()); }

	void headFromFile(const char* fn) {
		m_head = new(struct Head);
		filebuf fbuf;
        fbuf.open(fn, std::ios_base::in | std::ios_base::binary);
		fbuf.sgetn((char *)(m_head), sizeof(struct Head));
		fbuf.close();
	};
	void dumpHead() {
		const struct Head& s = *m_head;
		auto& [m0,m1,m2,m3,m4,m5,m6,m7,m8,m9,m10,m11] = s;
		cout << m0 << endl;
		cout << m1 << endl;
		cout << m2 << endl;
		cout << m3 << endl;
		cout << m4 << endl;
		cout << m5 << endl;
		cout << m6 << endl;
		cout << m7 << endl;
		cout << m8 << endl;
		cout << m9 << endl;
	};
	void dump() {
		dumpHead();
		cout << m_fileName << endl;
	};
	bool checkHead() {
		sha1 sha;
		char *szMsg = (char *)(m_head);
		sha.process_bytes(szMsg, 152);
		unsigned int digest[5];
		sha.get_digest(digest);
		stringstream s1,s2;
    	for (int i = 0; i< 5; ++i)
			s1 << hex << digest[i];
			
		for(int i = 0;i<40;i++) {
			//cout << m_head->sha1.sha1[i] << " ";
			s2 << m_head->sha1.sha1[i];
		}
		if(s1.str()!=s2.str()) {
			//cout<<"s1:"<<s1.str().c_str()<<" s2:"<<s2.str()<<endl;
			return false; 
		}
		name_generator ngen(usrtosNS);
		uuid u1 = ngen(s1.str().c_str());
		string stru1 = lexical_cast<string>(u1);
		auto fnLen = m_fileName.size();
		if(m_fileName.find(stru1) == string::npos) {
			//cout << stru1 << " " << m_fileName << endl;
			return false; 
		}
		return true;
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
			memset(&m,0,sizeof(m));
			cout << "fail to get lock" << endl;
			return false;
		}
	};
	bool checkLock() {
		return checkMutex(m_head->rd) && checkMutex(m_head->wr);
	};

	bool checkCP() {
		if(m_head->cpSize==0) return true;
		void *d = m_dataHandle.get_address();
		void *c = m_cpHandle.get_address();
		auto r = memcmp(d,c,m_head->cpSize);
		if(r==0)
			return true;
		cout << *(int *)d << " " << *(int *)c << " " << r <<endl; 
		return false;
	};

	bool attach(string fn) {
		struct block b;
		b.setFileName(fn);
		b.headFromFile();
		if(!b.checkHead()){
			cout << "error 0 " << fn << endl;
			return false;
		}
		try {
			m_fileName = fn;
			file_mapping mf(fn.c_str(), read_write);
			mf.swap(m_file);
			mapped_region mh( m_file
				, read_write
				, 0
				, b.m_head->metaSize
				);
			mh.mapped_region::swap(m_headHandle);
			m_head = (struct Head*)(m_headHandle.get_address());
			if(!checkHead()) {
				cout << "error 1" << endl;
				return false;
			}
			mapped_region*  pmt = new mapped_region(
				  m_file
				, read_write
				, b.m_head->metaSize
				, b.m_head->dataSize + b.m_head->cpSize
				);
			if(pmt->get_size()!=b.m_head->dataSize + b.m_head->cpSize) {
				cout << "error 2 " << pmt->get_size() << endl;
				return false;
			}
			m_base = pmt->get_address();
			
			delete pmt;
			
			mapped_region md( m_file
				, read_write
				, b.m_head->metaSize
				, b.m_head->dataSize
				, m_base
				);
			md.swap(m_dataHandle);

			if( b.m_head->cpSize>0 ){
				mapped_region mc( m_file
					, read_write
					, b.m_head->metaSize
					, b.m_head->cpSize
					, static_cast<char*>(m_base) + (size_t)b.m_head->dataSize
					);
				mc.swap(m_cpHandle);
			}
			m_attached = checkHead()&&checkCP()&&checkLock();
		}
		catch(interprocess_exception &ex){
			cout << "error 3" << endl;
			auto l = sizeof(m_head->rd);
			cout << ex.what() << " " << l << " " << &(m_head->rd) << " " << m_head << endl;
			cout << hex << *(long long*)(&(m_head->rd));
			cout << hex << *(long long*)(&(m_head->wr));
			cout << endl;
			auto rd = new(&(m_head->rd)) interprocess_mutex;
			auto wr = new(&(m_head->wr)) interprocess_mutex;
			cout << "affter init" << " ";
			cout << hex << *(long long*)(&(m_head->rd));
			cout << hex << *(long long*)(&(m_head->wr));
			cout << endl;
			return false;
		}
		return m_attached;
	};

	~block() {
	};
};