#define FUNCLASS capWorkersClearer
#include <capabilityAPI.hpp>
#include <usrtworker.hpp>
#include <glog.hpp>
using namespace usrtos;
struct ClearArgv {
	utime_t start;
	utime_t end;
};

int FUNCLASS::run( void *argv ) {
	struct structThread *my = (struct structThread*)argv;
	struct WorkerKeeperCTX *ctx = &(my->workers->ctx);
	SYSLOG = my->workers->_SYSLOG;
	if( !ctx->workers ) {		
		SYSLOG("in capWorkersClearer: thread %d can not get worker\n",my->id);
		return -1;
	}
	if( ctx->workers->tQueue() == nullptr ) {		
		SYSLOG("in capWorkersClearer: thread %d can not get Queue\n",my->id);
		return -1;
	}
	if(ctx->keeper_mutex.value() == false) {
		struct ClearArgv *cargv = static_cast<struct ClearArgv *>(argv); 
		uscoped_lock lock(ctx->keeper_mutex);
		int cleared = ctx->workers->tQueue()->clear(cargv->start,cargv->end);
		SYSLOG("in capWorkersClearer: clear task: %d",cleared);
	}
	else {		
		my->monitor.keeperLock++;
	}
	return 1;
}



