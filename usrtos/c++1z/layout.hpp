#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/sha1.hpp>

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
				,('rdlock',   c_int)       #192
				,('wrlock',   c_int)       #196
				]                          #200

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
	boost::interprocess::interprocess_mutex rd;
	boost::interprocess::interprocess_mutex wr;
};

struct block {
	struct Head* m_head;
	block(){};
	string m_fileName;
	file_mapping m_file;
	mapped_region m_headHandle;
	mapped_region m_dataHandle;
	uuid usrtosNS = lexical_cast<uuid>("8ea09e05-fd67-5949-a9ab-e722a3dae01c");
	bool m_attached = false;

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
	bool checkHead() {
		sha1 sha;
		char *szMsg = (char *)(m_head);
		sha.process_bytes(szMsg, 152);
		unsigned int digest[5];
		sha.get_digest(digest);
		stringstream s1,s2;
    	for (int i = 0; i< 5; ++i)
			s1 << hex << digest[i];

		//cout << m_head->sha1;
			
		for(int i = 0;i<40;i++) {
			//cout << m_head->sha1.sha1[i] << " ";
			s2 << m_head->sha1.sha1[i];
		}
		if(s1.str()!=s2.str()) {
			//cout<<"s1:"<<s1.str().c_str()<<" s2:"<<s2.str()<<endl;
			return false; //
		}
		name_generator ngen(usrtosNS);
		uuid u1 = ngen(s1.str().c_str());
		string stru1 = lexical_cast<string>(u1);
		auto fnLen = m_fileName.size();
		if(m_fileName.find(stru1) == string::npos) {
			//cout << stru1 << " " << m_fileName << endl;
			return false; //
		}
		return true;
	};
	/*
	template<class MemoryMappable>
   mapped_region(const MemoryMappable& mapping
                ,mode_t mode
                ,offset_t offset = 0
                ,std::size_t size = 0
                ,const void *address = 0
                ,map_options_t map_options = default_map_options);
	*/
	bool attach(string fn) {
		m_fileName = fn;
		file_mapping mf(fn.c_str(), read_write);
		mf.swap(m_file);
		mapped_region mh( m_file
			, read_write
			, 0
			, 4096
			);
		mh.mapped_region::swap(m_headHandle);
		m_head = (struct Head*)(m_headHandle.get_address());
		return checkHead();
	};
	~block() {
		if(m_attached)
			file_mapping::remove(m_fileName.c_str());
	};
};