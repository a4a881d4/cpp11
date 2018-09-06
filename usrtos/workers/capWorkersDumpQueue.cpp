#define FUNCLASS capWorkersDumpQueue
#include <capabilityAPI.hpp>
#include <usrtworker.hpp>

using namespace usrtos;

int FUNCLASS::run( void *argv ) {
  
  struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
  if( !ctx->workers )
    return -1;
  if( ctx->workers->tQueue() )
    ctx->workers->tQueue()->dumpHeap();
  return 0;
}



