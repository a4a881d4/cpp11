#define FUNCLASS capWorkersInitClock
#include <capabilityAPI.hpp>
#include <usrtworker.hpp>
#include <glog.hpp>

using namespace usrtos;

int FUNCLASS::run( void *argv ) {
	
	struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
	if( !ctx->workers )
		return -1;
	SYSLOG = ctx->workers->_SYSLOG;
	if( ctx->workers->m_c2s ) {
		ctx->workers->m_c2s.init();
		SYSLOG("Init Clock");
	} else {
		SYSLOG("Miss Clock");
	}
	return 0;
}



