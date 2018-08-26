#include "layout.hpp"
#include <sstream>

using namespace usrtos;

int main(int argc,char *argv[])
{
	struct CPBlock b;
	b.headFromFile(argv[1]);
	b.dumpHead();
	std::stringstream s;
    s << argv[1];
	b.setFileName(s.str());
	b.headFromFile();
	if(b.checkHead())
		std::cout << "Check head ok" << std::endl;
	if(b.attach(s.str()))
		std::cout << "Attach ok" << std::endl;
	
	if(b.checkHead())
		std::cout << "Check head ok" << std::endl;

	if(b.checkType("block"))
		std::cout << "Check type ok" << std::endl;

	if(b.checkMetaSize<4096>())
		std::cout << "Check meta size ok" << std::endl;

	MemoryBlockLayout::UsrtBlk m;
	if(m.attach(s.str())){
		std::cout << "Usrt block Attach ok" << std::endl;
		m.dump();
	}

	MemoryBlockLayout::UsrtMem um;
	if(um.attach(s.str())) {
		std::cout << "Usrt mem Attach ok" << std::endl;
		um.dump();
		auto pd = um.newLP<double>();
		auto p = um.newLP<int>(10,16);
		um.dump();
	}
	
	MemoryBlockLayout::UsrtFifo fm;
	if(fm.attach(s.str())){
		std::cout << "Usrt fifo Attach ok" << std::endl;
		std::cout << "Before alloc mem" << std::endl;
		fm.dump();
		auto pd = fm.newLP<double>();
		*pd = 1000.;
		std::cout << "After alloc mem" << std::endl;
		
		fm.dumpPoint();
		
		fm.local_reset();
		fm.push<double>(*pd);
		std::cout << "After push" << std::endl;
		
		fm.dumpPoint();
		
		auto lpc = fm.local_get<double>();
		std::cout << "After local get" << std::endl;
		if(lpc!=nullptr) {
			std::cout << *lpc << std::endl;
		}
		
		fm.dumpPoint();
		
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
		
		fm.dumpPoint();
	}
}