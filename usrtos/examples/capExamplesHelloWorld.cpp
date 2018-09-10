#define FUNCLASS capExamplesHelloWorld
#include <capabilityAPI.hpp>
#include <iostream>

using namespace usrtos;

int FUNCLASS::run( void *argv ) {
  
  std::cout << "Hello World" << std::endl;

  return 0;
}

