#define FUNCLASS capWorkersSetCapByKey
#include <capabilityAPI.h>

using namespace std;

int FUNCLASS::run( void *argv ) {
  
  struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
  if( !ctx->workers )
    return -1;
  int64 key= *(int64* )ctx->argv;
  ctx->workers->setCap( key );
  return 0;
}



