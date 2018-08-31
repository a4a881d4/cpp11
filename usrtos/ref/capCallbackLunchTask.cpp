typedef struct __callback_wait {
  raw_spinlock_t lock;
  int cnt;
  _callback_argv_t argv;
} _callback_wait_t;




#define FUNCLASS capCallBackLunchTask
#include <capabilityAPI.h>
#include <stdio.h>
#include <usrttype.h>
#include <USRTFifo.h>
#include <memory.h>
#include <MapMem.h>

using namespace std;


static void pushTask(USRTFifo *q, generalized_memory_t *task)
{
  void *vgp = q->allocMem(sizeof(generalized_memory_t));
  memcpy( vgp, task, sizeof(generalized_memory_t) );
  q->push(vgp);
}
static void dealCallBack(USRTFifo *q, _callback_argv_t *callback, task_t *ref )
{
  task_t *task;
  _callback_wait_t *wait;
  generalized_memory_t *multi;
  int ready;
  if( callback->mode!=extwait && callback->mode!=extmulti && callback->cnt != -1 ) {
    callback->cnt--;
    if( callback->cnt <= 0 )
      return;
  }
  switch( callback->mode ) {
    case none:  // do after me
      pushTask(q,&(callback->gp));
      break;
    case abss: // modify noE and noL
      task = (task_t*)G2L(&(callback->gp));
      task->noE = __getNow()+(int64)callback->delay;
      task->noL = task->noE;
      pushTask(q,&(callback->gp));
      break;
    case me: // delay callback->delay 
      if( ref==NULL ) ref=task;
      task = (task_t*)G2L(&(callback->gp));
      task->noE = ref->noE+(int64)callback->delay;
      task->noL = ref->noL+(int64)callback->delay;
      pushTask(q,&(callback->gp));
      break;
    case obj: // delay
      task = (task_t*)G2L(&(callback->gp));
      task->noE += (int64)callback->delay;
      task->noL += (int64)callback->delay;
      pushTask(q,&(callback->gp));
      break;
    case extwait: // Notification follow up task
      wait = (_callback_wait_t *)G2L(&(callback->gp));
      ready = 0;
      __raw_spin_lock(&(wait->lock));
      wait->cnt--;
      if( wait->cnt==0 )
        ready=1;
      __raw_spin_unlock(&(wait->lock));
      if( ready==1 )
        dealCallBack(q,&(wait->argv),NULL);
      break; // wake up follow task
    case extmulti:
      multi = (generalized_memory_t *)G2L(&(callback->gp));
      for( int i=0;i<callback->cnt;i++ )
        pushTask(q,multi+i);
      break;
    default:
      break;
  }
}
int FUNCLASS::run( void *argv ) {
  task_t *my = (task_t *)argv;
  _callback_argv_t *callback = &(my->callbackargv); 
  USRTFifo *queue = (USRTFifo *)callback->pQ;
  dealCallBack(queue, callback, my );
  return 0;
}



