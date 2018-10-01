#define FUNCLASS capWorkersHelloWorld
#include <capabilityAPI.hpp>
#include <usrtworker.hpp>

using namespace usrtos;

int FUNCLASS::run( void *argv ) {
  
  struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
  if( !ctx->workers )
    return -1;

  SYSLOG("Hello World");
  return 0;
}

