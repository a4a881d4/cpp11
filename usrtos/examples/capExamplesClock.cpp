#define FUNCLASS capExamplesClock
#include <capabilityAPI.hpp>
#include <iostream>
#include <timing/getnow.hpp>
#include <usrttype.hpp>
using namespace usrtos;

int FUNCLASS::run( void *argv ) {

	utime_t a = std::chrono::high_resolution_clock::now();
	std::cout << " system now: " << *((long long*)(&a)) << std::endl;
	std::cout << " cpu now: " << timing::CPUClock().getNow() << std::endl;
	return 0;
}
