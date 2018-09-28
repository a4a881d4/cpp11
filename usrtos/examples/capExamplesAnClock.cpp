#define FUNCLASS capExamplesAnClock
#include <capabilityAPI.hpp>
#include <iostream>
#include <fstream>

using namespace usrtos;
using namespace std;
int FUNCLASS::run( void *argv ) {

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
		std::cout << "total:" << tab[65533] << " ne:" << (long long int)sum << std::endl;
	}
	double sgm = sqrt(sumxx/sum - avg*avg);
	double sgm6 = avg+sgm*6.;
	double r = 0.;
	for(int i=sgm6;i<65500;i++) {
		r += tab[i];
	}
	std::cout << "sum   :" << (long long int)sum   << std::endl;
	std::cout << "avg   :" << avg   << std::endl;
	std::cout << "sgm   :" << sgm   << std::endl;
	std::cout << "sgm6  :" << sgm6  << std::endl;
	std::cout << "r/sum :" << r/sum << std::endl;
	std::cout << "out of 65500:" << tab[65500] << std::endl;
	std::cout << "max at:" << tab[65534] << " tab:" << tab[65535] << std::endl;
	ofstream back;
	back.open("/tmp/usrtos/delay.back",ios::binary);
	back.write((const char*)argv,65536*8);
	back.close();
	return 0;
}

