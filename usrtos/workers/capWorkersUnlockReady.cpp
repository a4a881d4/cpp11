#define FUNCLASS capWorkersUnlockReady
#include <capabilityAPI.h>

using namespace std;

int FUNCLASS::run( void *argv ) {
  
  struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
  if( !ctx->workers )
    return -1;
  ctx->workers->tQueue()->unLockReady();
  return 0;
}



