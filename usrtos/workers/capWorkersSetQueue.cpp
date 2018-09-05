#define FUNCLASS capWorkersSetQueue
#include <capabilityAPI.h>

using namespace std;

int FUNCLASS::run( void *argv ) {
  
  struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
  if( !ctx->workers )
    return -1;
  const char* qName = (const char*)ctx->argv;
  ctx->workers->setQueue( qName );
  return 0;
}



