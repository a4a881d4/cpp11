#define FUNCLASS capExamplesClock
#include <capabilityAPI.hpp>
#include <iostream>
#include <timing/clock.hpp>
#include <usrttype.hpp>
using namespace usrtos;

int FUNCLASS::run( void *argv ) {

	{
		auto a = timing::getWallNow();
		std::cout << " wall clock now: " << *((long long*)(&a)) << std::endl;
	}
	{
		auto a = std::chrono::high_resolution_clock::now();
		std::cout << " system now:     " << *((long long*)(&a)) << std::endl;
	}
	{
		auto a = timing::CPUClock().systime();
		std::cout << " cpu system time:" 
			<< *((long long*)(&a)) << std::endl;
	}
	
	return 0;
}
