#define FUNCLASS capWorkersDelCapByKey
#include <capabilityAPI.hpp>
#include <usrtworker.hpp>

using namespace usrtos;

int FUNCLASS::run( void *argv ) {
  
  struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
  if( !ctx->workers )
    return -1;
  uuid key= *(uuid* )ctx->argv;
  ctx->workers->removeBearerByKey( key );
  return 0;
}



