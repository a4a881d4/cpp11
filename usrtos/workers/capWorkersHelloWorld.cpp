#define FUNCLASS capWorkersHelloWorld
#include <capabilityAPI.hpp>
#include <iostream>
using namespace std;

int FUNCLASS::run( void *argv ) {
  
	cout << "Hello World" << endl;
	return 0;
}

