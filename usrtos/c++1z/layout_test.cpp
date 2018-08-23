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
	b.checkHead();
	std:cout << std::endl;
}