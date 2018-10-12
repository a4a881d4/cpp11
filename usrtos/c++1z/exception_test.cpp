#include <exceptions.hpp>
using namespace usrtos;

int t(int i) {
	if(i < 5) {
		std::cout << "i:" << i <<std::endl;
	} else {
		throw usrtos_exception("Test throw()");
	}
	return 0;
}
int main() {
	try {
		for(int i=0;i<10;i++) {
			t(i);
		}
	}
	catch(usrtos_exception& e) {
		std::cout << e.what() << std::endl;
	}
}