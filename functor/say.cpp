#include <iostream>
#include <string>
#include <string.h>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

using namespace std;

template <typename T>
struct Say
{
	Say(){};
	void operator()(T&& x){
		cout << "say: " << x << endl;

	};
};

struct sha1str {
	char sha1[40];
};

struct MyStruct {
	int a;
	int b;
	char name[10];
	char type[10];
	double c;
	double d;
	struct sha1str hash;
	boost::interprocess::interprocess_mutex rd;
	boost::interprocess::interprocess_mutex wr;

};

ostream& operator<<(ostream& os,const struct sha1str& h)
{
	char buf[50];
	memcpy(buf,&(h.sha1),40);
	buf[40]='\0';
	os<<buf;
	return os;
}

void print(const struct MyStruct& test )
{
	auto& [m0,m1,m2,m3,m4,m5,m6,m7,m8] = test;
	cout << m0 << endl;
	cout << m1 << endl;
	cout << m2 << endl;
	cout << m3 << endl;
	cout << m4 << endl;
	cout << m5 << endl;
	cout << m6 << endl;
	cout << endl;
}

int main()
{
	Say<string> say;
	say("ok");

	struct MyStruct s = {
		  1
		, 2
		, "I"
		, "person"
		, 1.0
		, 2.0
	};
	char str[] = "1234567890123456789012345678901234567890";
	memcpy(&(s.hash),str,40);

	print(s);
}