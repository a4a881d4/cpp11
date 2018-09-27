#define FUNCLASS capExamplesClock
#include <capabilityAPI.hpp>
#include <iostream>
#include <timing/clock.hpp>
#include <usrttype.hpp>
#include <chrono>

using namespace usrtos;

int FUNCLASS::run( void *argv ) {

	{
		auto a = timing::getWallNow().time_since_epoch();
		auto dd = std::chrono::duration_cast<timing::Second>(a);
		double ddd = dd.count();
		ddd -= (long long int)ddd;
		ddd *= 1e9;
		long long int id = (long long int)ddd;
		long long int *tab = (long long int *)argv;
		int iid = id%100000;
		if(iid < 65535)
			tab[iid]++;
		else
			tab[65535]++;
		if(tab[iid] > 10)
			std::cout << " wall clock now: " << id << " " << tab[iid] << std::endl;
	}
	// {
	// 	auto a = std::chrono::high_resolution_clock::now();
	// 	std::cout << " system now:     " << *((long long*)(&a)) << std::endl;
	// }
	// {
	// 	auto a = timing::CPUClock().systime();
	// 	std::cout << " cpu system time:" 
	// 		<< *((long long*)(&a)) << std::endl;
	// }
	
	return 0;
}
