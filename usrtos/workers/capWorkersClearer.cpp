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
	struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
	SYSLOG = ctx->workers->_SYSLOG;
	if( !ctx->workers ) {		
		SYSLOG("in capWorkersClearer: can not get worker");
		return -1;
	}
	if( ctx->workers->tQueue() == nullptr ) {		
		SYSLOG("in capWorkersClearer: thread can not get Queue");
		return -1;
	}
	struct ClearArgv *cargv = static_cast<struct ClearArgv *>(ctx->argv); 
	uscoped_lock lock(ctx->workers->ctx.keeper_mutex);
	auto q = ctx->workers->tQueue();
	size_t cleared = q->clear(cargv->start,cargv->end);
	
	std::stringstream s;
	s << "in capWorkersClearer: ";
	ctx->workers->m_c2s.dump(s);
	std::string logstr = s.str();
	SYSLOG.put(logstr);
	
	s.str("");
	s << "in capWorkersClearer: start: sys " << ctx->workers->m_c2s.toSys(cargv->start);
	s << " cpu " << cargv->start;
	logstr = s.str();
	SYSLOG.put(logstr);
	
	s.str("");
	s << "in capWorkersClearer: end:   sys " << ctx->workers->m_c2s.toSys(cargv->end);
	s << " cpu " << cargv->end;
	logstr = s.str();
	SYSLOG.put(logstr);
	
	s.str("");
	s << "in capWorkersClearer: clear task: " << cleared;
	logstr = s.str();
	SYSLOG.put(logstr);
	
	return 1;
}



