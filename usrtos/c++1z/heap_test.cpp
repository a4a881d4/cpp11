#include <cpblock.hpp>
#include "usrtheap.hpp"
#include "usrttask.hpp"

using namespace usrtos;
#include <sstream>
int main(int argc,char *argv[])
{
	struct CPBlock b;
	
	std::stringstream s;
    s << argv[1];
	b.setFileName(s.str());
	
	if(b.attach(s.str()))
		std::cout << "Attach ok" << std::endl;
	
	if(b.checkHead())
		std::cout << "Check head ok" << std::endl;

	if(b.checkType("block"))
		std::cout << "Check type ok" << std::endl;

	if(b.checkMetaSize<4096>())
		std::cout << "Check meta size ok" << std::endl;

	TaskHeap h(b);
	
	auto p = h.tm->newLP<task>();
	p->noE=1;
	p->noL=2;

	h.wait->insert(h.wait->LP2offset(p));
	h.wait->check(0);

	h.ready->insert(h.ready->LP2offset(p));
	h.ready->check(0);

}
