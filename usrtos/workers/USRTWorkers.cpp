#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <USRTWorkers.h>
#include <USRTHardTimer.h>

namespace std {
  static int64 USRTWorkers::keeperKey=md5first("capWorkersKeeper");
  void USRTWorkers::releaseThread() 
  {
    int i;
    for( i=0;i<threadNum;i++ )
      tids[i]->control=0;
  }
  int USRTWorkers::holdThread()
  {
    int i;
    int ready = 0;
    __raw_spin_lock(&tidsLock);
    for( i=0;i<threadNum;i++ )
      tids[i]->control=1;
    USRTHardTimer timer(100000);
    while( ready == 0 ) {
      ready=1;
      for( i=0;i<threadNum;i++ )
        if( tids[i]->state!=WAITING )
          ready=0;
      if( timer.expired() )
        break;
    }
    __raw_spin_unlock(&tidsLock);
    return ready;
  }
  void USRTWorkers::setQueue( const char *taskQueueName ) 
  {
    if( tQ==NULL ) {
      tQ = new USRTTaskQueue( taskQueueName );
      tQ->start();
    }
    else {
      if( holdThread() ) {
        delete tQ;
        tQ = new USRTTaskQueue( taskQueueName );
        tQ->start();
      }
      else {
        fprintf(stderr,"hold thread failure, can not assign new Task Queue\n");
        releaseThread();
      }
    }
  }
  void USRTWorkers::setCap( const char * name )
  {
     USRTCapabilityBearer *c = new USRTCapabilityBearer(name);
     if( c->isValid() )
      caps[c->getKey()]=c; 
  }
  void USRTWorkers::setCap( int64 key )
  {
     USRTCapabilityBearer *c = new USRTCapabilityBearer(key);
     if( c->isValid() )
      caps[key]=c; 
  }
  void USRTWorkers::setDefaultKeeper( int64 key )
  {
    keeperKey = key;
    caps[keeperKey]=new USRTCapabilityBearer(keeperKey);
      if( !caps[keeperKey]->isValid() )
        fprintf(stderr,"Can not find default keeper\n");
  }  
  USRTWorkers::USRTWorkers( const char* name )
  {
    newUSRTTask( name );
    USRTTask::start();
    tids = NULL;
    threadNum = 0;
    ctx.workers = this;
    __raw_spin_unlock(&(ctx.keeperLock));
    tQ=NULL;
    control = 0;
  }
  int USRTWorkers::stopThread( int k )
  {
    tids[k]->control=-1;
    if( tids[k]->tid )
      pthread_join( tids[k]->tid, NULL );
    tids[k]->tid=0;
    delete tids[k];
    tids[k]=NULL;
  }
  int USRTWorkers::runThread( int k )
  {
    tids[k] = new struct structThread;
    memset( tids[k], 0, sizeof(struct structThread) );
    tids[k]->id = k;
    tids[k]->workers = this;
    pthread_create( &(tids[k]->tid), NULL, worker, tids[k] );
    
  }
  USRTWorkers::~USRTWorkers()
  {
    
    if( tids ) {
      int i;
      for( i=0;i<threadNum;i++ )
        stopThread(i);
      delete tids;
    }
    if( tQ )
      delete tQ;
  }
  void USRTWorkers::start(int n)
  {
    int i;
    fprintf(stderr,"Start thread %d(%d)\n",n,threadNum);
    if( threadNum>n ) {
      struct structThread **ltids = new struct structThread *[n];
      for( i=0;i<n;i++ )
        ltids[i] = tids[i];
      for( i=n;i<threadNum;i++ )
        stopThread(i);
      struct structThread **ttids = tids;
      tids = ltids;
      delete ttids;
      threadNum=n;
    }
    else
    {
      struct structThread **ltids = new struct structThread *[n];
      for( i=0;i<threadNum;i++ )
        ltids[i] = tids[i];
      struct structThread **ttids = tids;
      tids = ltids;
      delete ttids;
      for( i=threadNum;i<n;i++ )
        runThread(i);
      threadNum=n;  
    }
  }
  USRTCapabilityBearer *USRTWorkers::getBearerByKey( int64 key )
  {
    map<int64,USRTCapabilityBearer *>::iterator iter;
    iter = caps.find( key );
    if( iter != caps.end() ) 
      return iter->second;
    else
      return NULL; 
  }
  void USRTWorkers::removeBearerByKey( int64 key )
  {
    map<int64,USRTCapabilityBearer *>::iterator iter;
    iter = caps.find( key );
    if( iter != caps.end() ) {
      USRTCapabilityBearer * capToBeDel = iter->second;
      caps.erase (iter);
      sleep(1);
      delete capToBeDel;
    }
  }
  void USRTWorkers::listCaps()
  {
    map<int64,USRTCapabilityBearer *>::iterator iter;
    for( iter=caps.begin();iter!=caps.end();iter++ ) {
      if( iter->second->isValid() )
        fprintf(stderr,"%llx(%s)\n",iter->second->getKey(),iter->second->getName());
    }  
  }    
  static void USRTWorkers::worker( void *argv )
  {
    struct structThread *my = (struct structThread *)argv;
    fprintf(stderr,"Thread %d is start\n", my->id);
    my->sysid=(long int)syscall(__NR_gettid);
    while( my->control!=-1 ) {
      while( my->control==1 ) {
        my->state=WAITING;
      }
      my->state=RUNNING;
      task_t *t = my->workers->pop();
      USRTCapabilityBearer *bearer;
      if( t != NULL ) {
        my->monitor.run++;
        int64 capKey = t->key;
        bearer = my->workers->getBearerByKey(capKey);
        if( bearer != NULL ) {
          bearer->runGP( &(t->argv) );
          if( t->callback ) {
            bearer = my->workers->getBearerByKey(t->callback);
            if( bearer != NULL ) {
              my->monitor.callback++;
              t->callbackargv.pQ = my->workers->tQueue();
              bearer->runLP( t );
            }
          }
        }
      }
      else /*if( my->id==0)*/ {
        my->monitor.keeper++;
        my->state=KEEPER;
        bearer = my->workers->getBearerByKey(keeperKey);
        if( bearer!=NULL )
          bearer->runLP( my );
      }
    }
    my->state=EXITING;
    fprintf(stderr,"Thread %d is stop\n", my->id);
    pthread_exit(NULL);
  }
  task_t* USRTWorkers::pop()
  {
    if( tQ != NULL )
      return tQ->pop();
    else
      return NULL;  
  }
  static void USRTWorkers::mainWorker( void *argv )
  {
    USRTWorkers *my = (USRTWorkers *)argv;
    USRTCapabilityBearer *bearer;
    struct mainWorkerCTX mCtx;
    mCtx.workers = my;
    
    while( my->control==0 ) {
      if( my->len()>0 ) {
        task_t *t = my->get();
        bearer = my->getBearerByKey(t->key);
        if( bearer==NULL ) {
          my->setCap(t->key);
          bearer = my->getBearerByKey(t->key);
        }
        if( bearer!=NULL ) {
          if( t->ID==0LL ) {
            mCtx.argv=G2L(&(t->argv));
            bearer->runLP( &(mCtx) );
          }
          else
            bearer->runGP(&(t->argv));
        }
      }
      else
        sleep(1);
    }
    delete my;     
  }
};


