#define FUNCLASS capWorkersKeeper
#include <capabilityAPI.hpp>
#include <usrtworker.hpp>

using namespace usrtos;

int FUNCLASS::run( void *argv ) {
  struct structThread *my = (struct structThread*)argv;
  struct WorkerKeeperCTX *ctx = &(my->workers->ctx);
  if( !ctx->workers ) {    
	  printf("you are into Worker Keeper\n");
	  printf("thread %d can not get worker\n",my->id);
	  return -1;
	}
  if( ctx->workers->tQueue() == nullptr ) {    
	  printf("you are into Worker Keeper\n");
	  printf("thread %d can not get Queue\n",my->id);
	  return -1;
	}
  if(ctx->keeper_mutex.value() == false) {
    uscoped_lock lock(ctx->keeper_mutex);
    ctx->workers->tQueue()->update();
  }
  else {    
	  my->monitor.keeperLock++;
	}
	return 1;
}



