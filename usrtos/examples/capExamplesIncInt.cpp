#define FUNCLASS capExamplesIncInt
#include <capabilityAPI.hpp>
#include <iostream>

using namespace usrtos;

int FUNCLASS::run( void *argv ) {

	int *a = (static_cast<int *>(argv));
	int b = (*a)++;
	if((b%100)==0)
		std::cout << " I am in Inc Int with args " << b << std::endl;
	return 0;
}

