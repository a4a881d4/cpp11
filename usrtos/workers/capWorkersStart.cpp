#define FUNCLASS capWorkersStart
#include <capabilityAPI.h>

using namespace std;

int FUNCLASS::run( void *argv ) {
  
  struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
  if( !ctx->workers )
    return -1;
  int num = *(int *)ctx->argv;
  ctx->workers->start( num );
  return 0;
}



