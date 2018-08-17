/*
	AST
*/
#include <iostream>
#include <string>
#include <boost/ptr_container/ptr_vector.hpp>
using namespace std;
using namespace boost;

class Func {
	public:
		void* m_args;
		Func(string&& fn):m_name(fn){};
		void fake_do(string&& pre) {
			cout << pre << "fake call " << m_name << endl;

		};
	private:
		string& m_name;
		

};

struct VMV {
	string& m_name;
	VMV(string&& name):m_name(name){};
	void bind(Func& f, ptr_vector<VMV>& args ){
		m_f = &f;
		m_args = &args; 
	};
	void fake_do(string&& pre){
		m_f->fake_do(pre+m_name+"->");
	}
private:
	ptr_vector<VMV> *m_args = nullptr;
	Func* m_f = nullptr;

};

int main()
{
	struct fftargs {
		int length;
	};
	Func f("fft");
	ptr_vector<VMV> a;
	VMV fo("ffto");
	fo.bind(f,a);
	fo.fake_do("main->");
}