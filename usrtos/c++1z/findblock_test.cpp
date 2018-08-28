#include "findblock.hpp"
#include <sstream>

using namespace usrtos;

int main(int argc,char *argv[])
{
	FindBlock fb;
	
	std::stringstream s;
    s << argv[1];
	fb.setDir(s.str());
	fb.list();
	fb.dumpHead();
	fb.attach();
	fb.dump();
}