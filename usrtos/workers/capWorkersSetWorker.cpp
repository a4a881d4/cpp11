#define FUNCLASS capWorkersSetWorker
#include <capabilityAPI.hpp>
#include <usrtworker.hpp>
#include <workerhelper.hpp>

using namespace usrtos;

int FUNCLASS::run( void *argv ) {
	
	struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
	if( !ctx->workers )
		return -1;

	uuid key;
	memcpy(&key,ctx->argv,sizeof(uuid));
	ctx->workers->setDefaultWorker( key );
	return 0;
}
#undef FUNCLASS


