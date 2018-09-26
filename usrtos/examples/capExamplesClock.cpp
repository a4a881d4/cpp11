#define FUNCLASS capExamplesClock
#include <capabilityAPI.hpp>
#include <iostream>
#include <timing/clock.hpp>
#include <usrttype.hpp>
using namespace usrtos;

int FUNCLASS::run( void *argv ) {

	auto a = std::chrono::high_resolution_clock::now();
	std::cout << " system now: " << *((long long*)(&a)) << std::endl;
	std::cout << " cpu now: " << timing::CPUClock().now() << std::endl;
	return 0;
}
