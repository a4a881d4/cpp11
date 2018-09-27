#define FUNCLASS capExamplesAnClock
#include <capabilityAPI.hpp>
#include <iostream>

using namespace usrtos;

int FUNCLASS::run( void *argv ) {

	long long int *tab = (long long int *)argv;
	double sumx = 0.;
	double sumxx = 0.;
	double sum = 0.;
	for(int i=0;i<65536;i++) {
		if(tab[i]>10)
			std::cout << ">10 :" << tab[i] << " " << i << std::endl;
		sumx += (double)(tab[i])*i;
		sumxx += (double)(tab[i])*i*i;
		sum += (double)(tab[i]);
	}
	double avg = sumx/sum;
	double sgm = sqrt(sumxx/sum - avg*avg);
	double sgm6 = avg+sgm*6.;
	double r = 0.;
	for(int i=sgm6;i<65536;i++) {
		r += tab[i];
	}
	std::cout << "sum   :" << sum   << std::endl;
	std::cout << "avg   :" << avg   << std::endl;
	std::cout << "sgm   :" << sgm   << std::endl;
	std::cout << "sgm6  :" << sgm6  << std::endl;
	std::cout << "r/sum :" << r/sum << std::endl;
	
	return 0;
}
