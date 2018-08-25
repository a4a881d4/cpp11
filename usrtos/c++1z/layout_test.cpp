#include "layout.hpp"
#include <sstream>

int main(int argc,char *argv[])
{
	struct block b;
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
	if(m.attach(s.str()))
		std::cout << "Usrt block Attach ok" << std::endl;

	m.dump();

	MemoryBlockLayout::UsrtMem um;
	if(um.attach(s.str()))
		std::cout << "Usrt mem Attach ok" << std::endl;

	um.dump();
}