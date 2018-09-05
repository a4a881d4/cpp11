#define FUNCLASS capWorkersDumpQueue
#include <capabilityAPI.h>

using namespace std;

int FUNCLASS::run( void *argv ) {
  
  struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
  if( !ctx->workers )
    return -1;
  if( ctx->workers->tQueue() )
    ctx->workers->tQueue()->dumpHeap();
  return 0;
}



