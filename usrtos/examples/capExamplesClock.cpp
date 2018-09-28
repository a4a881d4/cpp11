#define FUNCLASS capExamplesClock
#include <capabilityAPI.hpp>
#include <iostream>
#include <timing/clock.hpp>
#include <usrttype.hpp>
#include <chrono>
#include <fstream>

using namespace usrtos;
using namespace std;
int an(void *argv) {
	ofstream tout;
	tout.open("/tmp/usrtos/test.result",ios::app);
	long long int *tab = (long long int *)argv;
	double sumx = 0.;
	double sumxx = 0.;
	double sum = 0.;
	for(int i=0;i<65500;i++) {
		if(tab[i] < tab[65533]) {
			sumx += (double)(tab[i])*i;
			sumxx += (double)(tab[i])*i*i;
			sum += (double)(tab[i]);
		} else {
			std::cout << "error @" << i << " tab:" << tab[i] << std::endl;
		}
	}
	double avg = sumx/sum;
	if((long long int)sum != tab[65533]) {
		tout << "total:" << tab[65533] << " ne:" << sum << std::endl;
	}
	double sgm = sqrt(sumxx/sum - avg*avg);
	double sgm6 = avg+sgm*6.;
	double r = 0.;
	for(int i=sgm6;i<65500;i++) {
		r += tab[i];
	}
	tout << "sum   :" << sum   << endl;
	tout << "avg   :" << avg   << endl;
	tout << "sgm   :" << sgm   << endl;
	tout << "sgm6  :" << sgm6  << endl;
	tout << "r/sum :" << r/sum << endl;
	tout << "out of 65500:" << tab[65500] << endl;
	tout << "max at:" << tab[65534] << " tab:" << tab[65535] << endl;
	tout.close();
	return 0;
}
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
		if((tab[65533]%10000) == 0) {
			std::cout << " wall clock now[" << tab[65533] << "]: " << id << " " << tab[iid] << std::endl;
			std::cout << " max:" << tab[65535] << "@"<< tab[65534] << std::endl;
		}
		if((tab[65533]%60000) == 0) {
			an(argv);
		}
	}
	
	return 0;
}
