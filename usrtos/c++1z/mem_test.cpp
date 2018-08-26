#include <cpblock.hpp>
#include "usrtmem.hpp"

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

	Mem<0x200> m(b);
	std::cout << "before alloc" << std::endl;
	m.dump();
	auto p = m.newLP<double>();
	std::cout << "after alloc" << std::endl;
	m.dump();

	Fifo<0x200,0x1000> fm(b);
	std::cout << "Before alloc mem" << std::endl;
	fm.dump();
	auto pd = fm.newLP<double>();
	*pd = 1000.;
	std::cout << "After alloc mem" << std::endl;
	
	fm.dump();
	
	fm.local_reset();
	fm.push<double>(*pd);
	std::cout << "After push" << std::endl;
	
	fm.dump();
	
	auto lpc = fm.local_get<double>();
	std::cout << "After local get" << std::endl;
	if(lpc!=nullptr) {
		std::cout << *lpc << std::endl;
	}
	
	fm.dump();
	
	while(1) {
		auto pc = fm.get<double>();
		if(pc!=nullptr) {
			std::cout << *pc << std::endl;
		} else {
			std::cout << "fifo empty" << std::endl;
			break;
		}
	}
	std::cout << "After get" << std::endl;
	
	fm.dump();
}
