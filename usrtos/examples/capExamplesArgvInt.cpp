#define FUNCLASS capExamplesArgvInt
#include <capabilityAPI.hpp>
#include <iostream>

using namespace usrtos;

int FUNCLASS::run( void *argv ) {

	int a = *(static_cast<int *>(argv));
	std::cout << " I am in Argv Int with args " << a << std::endl;
	std::string ms = UserHelper::dumpMem(static_cast<void*>(&meta),a);
	std::cout << ms << std::endl;
	return 0;
}

