#define FUNCLASS capWorkersSetDefaultKeeper
#include <capabilityAPI.h>

using namespace std;

int FUNCLASS::run( void *argv ) {
  
  struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
  if( !ctx->workers )
    return -1;
  int64 key= md5first("capWorkersKeeper");
  ctx->workers->setDefaultKeeper( key );
  return 0;
}



