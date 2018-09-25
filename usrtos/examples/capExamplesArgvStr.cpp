#define FUNCLASS capExamplesArgvStr
#include <capabilityAPI.hpp>
#include <iostream>

using namespace usrtos;

int FUNCLASS::run( void *argv ) {

	char *a = static_cast<char *>(argv);
	std::cout << " I am in Argv Str with argv: " << a << std::endl;
	return 0;
}

