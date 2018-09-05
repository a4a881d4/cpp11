#define FUNCLASS capWorkersSetCap
#include <capabilityAPI.h>

using namespace std;

int FUNCLASS::run( void *argv ) {
  
  struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
  if( !ctx->workers )
    return -1;
  const char* cap= (const char*)ctx->argv;
  ctx->workers->setCap( cap );
  return 0;
}



