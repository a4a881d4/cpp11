#define FUNCLASS capWorkersListCap
#include <capabilityAPI.hpp>
#include <usrtworker.hpp>

using namespace usrtos;

int FUNCLASS::run( void *argv ) {
  
  struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
  if( !ctx->workers )
    return -1;
  ctx->workers->listCaps();
  return 0;
}
#undef FUNCLASS


