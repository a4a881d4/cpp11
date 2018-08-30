#include "findblock.hpp"

using namespace usrtos;

int main(int argc,char *argv[])
{
	
	std::string dir(argv[1]);

	FindBlock fb(dir);
	
	auto heads = fb.list();
	std::cout << "heads type is " << typeid(heads).name() << std::endl;
	fb.dumpHead(heads);
	auto blocks = fb.attach(heads);
	std::cout << "blocks type is " << typeid(blocks).name() << std::endl;
	fb.dump(blocks);
}