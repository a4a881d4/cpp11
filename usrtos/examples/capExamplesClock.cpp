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
		if(iid > 65500)
			iid = 65500;
		tab[iid]++;
		if(tab[iid] > tab[65535]) {
			tab[65535] = tab[iid];
			tab[65534] = iid;
		}
		tab[65533]++;
		if((tab[65533]%1000) == 0) {
			std::cout << " wall clock now[" << tab[65533] << "]: " << id << " " << tab[iid] << std::endl;
			std::cout << " max:" << tab[65535] << "@"<< tab[65534] << std::endl;
		}
	}
	
	return 0;
}
