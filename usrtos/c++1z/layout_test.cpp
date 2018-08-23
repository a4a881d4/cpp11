#include "layout.hpp"

int main(int argc,char *argv[])
{
	struct block b;
	b.headFromFile(argv[1]);
	b.dumpHead();
}