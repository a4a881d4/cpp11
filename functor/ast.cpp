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
		string& m_funcName;
		Func(string fn):m_funcName(fn){};
		void fake_do() {
			cout << "fake call " << m_funcName << endl;

		};


};

struct VMV {
	VMV(){};
	void bind(Func& f, ptr_vector<VMV>& args ){
		m_f = &f;
		m_args = &args; 
	};
	void fake_do(){
		m_f->fake_do();
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
	VMV fo;
	fo.bind(f,a);
	fo.fake_do();
}