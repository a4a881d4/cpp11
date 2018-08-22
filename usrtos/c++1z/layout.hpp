#include <boost/interprocess/sync/interprocess_mutex.hpp>

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
struct Head {
	char name[32];
	char type[32];
	long long metaSize;
	long long dataSize;
	long long cpSize;
	double longitude;
	double latitude;
	double altitude;
	char version[40];
	char sha1[40];
	boost::interprocess::interprocess_mutex rd;
	boost::interprocess::interprocess_mutex wr;
};

struct block {
	struct Head* mhead;
	block(){};
	headFromFile(const char* fn) {
		mhead = new(struct Head);
		std::filebuf fbuf;
        fbuf.open(fn, std::ios_base::in | std::ios_base::binary);
		fbuf.sgetn(mhead, sizeof(struct Head));
		fbuf.close();
	};
	dumpHead() {
		
	}
}