#define FUNCLASS capWorkersTracer
#include <capabilityAPI.h>


using namespace std;

int FUNCLASS::run( void *argv ) {
  struct structThread *my = (struct structThread*)argv;
  struct WorkerKeeperCTX *ctx = &(my->workers->ctx);
  if( !ctx->workers ) {    
	  printf("you are into Worker Keeper\n");
	  printf("thread %d can not get worker\n",my->id);
	  return;
	}
  if( ctx->workers->tQueue() == NULL ) {    
	  printf("you are into Worker Keeper\n");
	  printf("thread %d can not get Queue\n",my->id);
	  return;
	}
  if( ctx->keeperLock.slock==1 ) {
    __raw_spin_lock(&(ctx->keeperLock));
    int length;
    while( (length=ctx->workers->tQueue()->len())>0 ) {
      
//      fprintf(stderr,"Queue has %d task\n",length);
      generalized_memory_t *gpTask = (generalized_memory_t *)ctx->workers->tQueue()->get();
      if( gpTask != NULL ) {
        _trace_t *trace = formGpTask(gpTask);
        if( trace !=NULL ) {
          trace->keeper=my->sysid;
          trace->pop=__getNow();
        }
        int put = ctx->workers->tQueue()->insert( gpTask );
        if( put == -1 ) {
//          fprintf(stderr,"Wait Heap is full\n");
//          ctx->workers->tQueue()->push((void *)gpTask);
        }
        if( put == -2 ) {
          fprintf(stderr,"Task is invalid\n");
        }  
      }
    }
    ctx->workers->tQueue()->updateTrace(my);
    __raw_spin_unlock(&(ctx->keeperLock));    
  }
  else {    
	  my->monitor.keeperLock++;
	}
	return 1;
}



