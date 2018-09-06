#define FUNCLASS capWorkersSetCap
#include <capabilityAPI.hpp>
#include <usrtworker.hpp>

using namespace usrtos;


int FUNCLASS::run( void *argv ) {
  
  struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
  if( !ctx->workers )
    return -1;
  const char* cap= (const char*)ctx->argv;
  ctx->workers->setCap( cap );
  return 0;
}



