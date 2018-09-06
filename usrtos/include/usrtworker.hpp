#pragma once

#include <map>
#include <string>
#include <pthread.h>
#include <capability.hpp>
#include <usrttype.hpp>
#include <usrttask.hpp>
#include <capabilityBearer.hpp>
#include <unistd.h>
#include <sys/syscall.h>
#include <findblock.hpp>
#include <layout.hpp>
#include <usrthardtimer.hpp>
#include <pthread.h>

using namespace std;

namespace usrtos {
  class UsrtWorkers;

  enum threadState { CREATING, RUNNING, WAITING, KEEPER, EXITING };
  
  struct structThreadMonitor {
    unsigned long long int run;
    unsigned long long int keeper;
    unsigned long long int keeperLock;
    unsigned long long int callback;
  };
    
  struct structThread {
    pthread_t tid;
    volatile int control;
    volatile enum threadState state;
    int id;
    struct structThreadMonitor monitor;
    UsrtWorkers *workers;
    long int sysid;
  };

  struct WorkerKeeperCTX {
    UsrtWorkers *workers;
    umutex keeper_mutex;
    };

  struct mainWorkerCTX {
    UsrtWorkers *workers;
    void *argv;
    };

  class UsrtWorkers {
    private:
      void dumpMonitor( struct structThreadMonitor& m ) {
        fprintf(stderr,"R:%lld K:%lld L:%lld CB:%lld\n"
          , m.run
          , m.keeper
          , m.keeperLock
          , m.callback
          );
      };
    public:
      void dumpThread( struct structThread *t ) {
        fprintf(stderr,"thread %d state:%d control:%d tid:%ld\n"
          , t->id
          , (int)t->state
          , t->control
          , t->sysid
          );
        dumpMonitor( t->monitor );
      };
      
      void dumpThread() {
        for(int i=0;i<threadNum;i++ )
          dumpThread( tids[i] );
          m_taskq->dumpHeap();
      };
    private:
      uuid keeperKey;
      map<uuid,UsrtCapabilityBearer *> caps;
      umutex tidmutex;
      int threadNum;
      int control;
      struct structThread **tids;
      void releaseThread() {
        int i;
        for( i=0;i<threadNum;i++ )
          tids[i]->control=0;
      };
      int holdThread() {
        int i;
        int ready = 0;
        uscoped_lock lock(tidmutex);

        for(i = 0;i < threadNum;i++)
          tids[i]->control=1;
        UsrtHardTimer timer(1000);
        while(ready == 0) {
          ready=1;
          for(i = 0;i < threadNum;i++)
            if(tids[i]->state != WAITING)
              ready=0;
          if(timer.expired())
            break;
        }
        return ready;
      };

      int stopThread(int k) {
        tids[k]->control=-1;
        if(tids[k]->tid)
          pthread_join( tids[k]->tid, NULL);
        tids[k]->tid=0;
        delete tids[k];
        tids[k]=nullptr;
        return k;
      };
      
      int runThread( int k ) {
        tids[k] = new struct structThread;
        memset( tids[k], 0, sizeof(struct structThread) );
        tids[k]->id = k;
        tids[k]->workers = this;
        pthread_create(&(tids[k]->tid), NULL, UsrtWorkers::worker, tids[k]);
        return k;
      };
      template<typename T>
      T* bindBlock(string name) {
          auto it = m_memName.find(name);
          T *r;
          if(it != m_memName.end() ) {
            r = new T(*(m_blocks[it->second]));
          }
          else {
            cerr << "bind block " << name << " failure" << endl;
            r = nullptr;
          }
          return r;
      };
    public:
      typedef TaskHeap UsrtTask;

      struct WorkerKeeperCTX ctx;
      
      map<uuid,CPBlock*>  m_blocks;
      map<string,uuid>   m_memName;
      Layout::UsrtFifo  *m_capFifo;
      Layout::UsrtFifo *m_taskFifo;
      UsrtTask            *m_taskq;
      Layout::UsrtMem    *m_memory;

      UsrtWorkers( const char* dir ) {
        FindBlock fb(dir);
        auto heads = fb.list();
        auto blocks = fb.attach(heads);
        for(auto it=blocks.begin();it != blocks.end(); ++it) {
          cerr << it->second->getName() << endl;
          auto key = it->second->getKey();
          m_blocks[key] = it->second;
          m_memName[it->second->getName()] = key;
        }
        
          m_capFifo = bindBlock<Layout::UsrtFifo>("capFifo");
          m_taskFifo = bindBlock<Layout::UsrtFifo>("taskFifo");
          m_taskq = bindBlock<UsrtTask>("taskq");
          m_memory = bindBlock<Layout::UsrtMem>("memory");
      };

      UsrtTask *tQueue() { 
        return m_taskq; 
      };
      
      void defaultKeeper( struct WorkerKeeperCTX *ctx );
      
      template <typename T>
      T* G2L(CPBlock::GP& gp) {
        if(m_blocks.count(gp.id) > 0)
          return m_blocks[gp.id]->GP2LP<T>(gp);
        else
          return nullptr;
      };

      template <typename T>
      bool L2G(CPBlock::GP& gp, T* p) {
        for(auto it = m_blocks.begin();it != m_blocks.end();++it) {
          if(it->second->validAddress(p)) {
            return it->second->LP2GP(gp,p);
          }
        }
        return false;
      };

      UsrtCapabilityBearer *getBearerByKey( uuid key ) {
        auto iter = caps.find( key );
        if( iter != caps.end() ) 
          return iter->second;
        else
          return nullptr; 
      };
      
      void removeBearerByKey( uuid key ) {
        auto iter = caps.find( key );
        if(iter != caps.end()) {
          UsrtCapabilityBearer * capToBeDel = iter->second;
          caps.erase (iter);
          sleep(1);
          delete capToBeDel;
        }
      };

      ~UsrtWorkers() {
        if(tids) {
          int i;
          for(i = 0;i < threadNum;i++)
            stopThread(i);
          delete tids;
        }
      };

      bool setCap(const char* name) {
        UsrtCapabilityBearer *c = new UsrtCapabilityBearer(name);
        if(c->isValid()) {
          caps[c->getKey()]=c;
          return true; 
        }
        return false;
      };
      
      bool setCap(uuid key) {
        UsrtCapabilityBearer *c = new UsrtCapabilityBearer(key);
        if(c->isValid()) {
          caps[key]=c;
          return true;
        }
        return false;
      };
      
      void start( int n ) {
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
      };
      
      static void* worker(void *argv) {
        struct structThread *my = (struct structThread *)argv;
        fprintf(stderr,"Thread %d is start\n", my->id);
        my->sysid=(long int)pthread_self();
        while( my->control!=-1 ) {
          while( my->control==1 ) {
            my->state=WAITING;
          }
          my->state=RUNNING;
          task *t = my->workers->m_taskq->pop();
          UsrtCapabilityBearer *bearer;
          if(t != nullptr) {
            my->monitor.run++;
            auto capKey = t->key;
            bearer = my->workers->getBearerByKey(capKey);
            if(bearer != nullptr) {
              bearer->runLP(
                static_cast<void*>
                (my->workers->G2L<char>(t->argv))
                );
              if(t->callbackargv.mode != CBMode::nocallback) {
                bearer = my->workers->getBearerByKey(t->callback);
                if(bearer != nullptr) {
                  my->monitor.callback++;
                  t->callbackargv.workers = my->workers;
                  bearer->runLP(t);
                }
              }
            }
          }
          else /*if( my->id==0)*/ {
            my->monitor.keeper++;
            my->state=KEEPER;
            bearer = my->workers->getBearerByKey(my->workers->keeperKey);
            if( bearer!=nullptr )
              bearer->runLP(my);
          }
        }
        my->state = EXITING;
        fprintf(stderr,"Thread %d is stop\n", my->id);
        pthread_exit(NULL);
        return nullptr;
      };
      
      void mainWorker()
      {
        UsrtCapabilityBearer *bearer;
        struct mainWorkerCTX mCtx;
        mCtx.workers = this;
        
        while( this->control==0 ) {
          if( this->m_taskFifo->len()>0 ) {
            task *t = this->m_taskFifo->get<task>();
            bearer = this->getBearerByKey(t->key);
            if( bearer==nullptr ) {
              if(this->setCap(t->key))
                bearer = this->getBearerByKey(t->key);
            }
            if( bearer != NULL ) {
              if( t->ID == 0LL ) {  // system task
                mCtx.argv = static_cast<void*>(G2L<char>(t->argv));
                bearer->runLP(&(mCtx));
              }
              else
                bearer->runLP(
                  static_cast<void*>
                  (G2L<char>(t->argv))
                  );
            }
          }
          else
            sleep(1);
        }
      };
          
      void setDefaultKeeper(uuid key) {
        if(!setCap(key))
          fprintf(stderr,"Can not find default keeper\n");
        else
          keeperKey = key;
      };
      
      void workerExit() { 
        control=-1; 
      };
      
      void listCaps() {
        for(auto iter = caps.begin();iter != caps.end();++iter) {
          if(iter->second->isValid()) {
            auto key = iter->second->getKey();
            cerr << UsrtKey::key2string(key) 
              << '(' << iter->second->getName() << ")" << endl;
          }
         } 
      };

      void dumpQueue() { 
        m_taskq->dumpHeap(); 
      };
      void listThread() {};
  };
}; // namespace usrtos
