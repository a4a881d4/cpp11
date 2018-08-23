#include <boost/interprocess/sync/interprocess_mutex.hpp>
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
	struct Head* mhead;
	block(){};
	string mfileName;
	uuid usrtosNS = lexical_cast<uuid>("8ea09e05-fd67-5949-a9ab-e722a3dae01c");
	void setFileName(string fn) { mfileName = fn; };
	
	void headFromFile() { headFromFile(mfileName.c_str()); }

	void headFromFile(const char* fn) {
		mhead = new(struct Head);
		filebuf fbuf;
        fbuf.open(fn, std::ios_base::in | std::ios_base::binary);
		fbuf.sgetn((char *)(mhead), sizeof(struct Head));
		fbuf.close();
	};
	void dumpHead() {
		const struct Head& s = *mhead;
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
		char *szMsg = (char *)(mhead);
		sha.process_bytes(szMsg, 152);
		unsigned int digest[5];
		sha.get_digest(digest);
		stringstream s1,s2;
    	for (int i = 0; i< 5; ++i)
			s1<< hex << digest[i];
		for(int i;i<40;i++)
			s2<<mhead->sha1.sha1[i];
		if(s1.str()!=s2.str())
			return false; //cout<<s1.str().c_str()<<s2.str()<<endl;
		name_generator ngen(usrtosNS);
		uuid u1 = ngen(s1.str().c_str());
		string stru1 = lexical_cast<string>(u1);
		auto fnLen = mfileName.size();
		if(mfileName.find(stru1) == string::npos)
			return false; //cout << stru1 << " " << mfileName << endl;
		return true;
	};
};