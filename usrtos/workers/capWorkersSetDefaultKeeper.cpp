#define FUNCLASS capWorkersSetDefaultKeeper
#include <capabilityAPI.hpp>
#include <usrtworker.hpp>
#include <workerhelper.hpp>

using namespace usrtos;

int FUNCLASS::run( void *argv ) {
	
	struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
	if( !ctx->workers )
		return -1;
	uuid key = WorkerHelper::cap2key(string("capWorkersKeeper"));

	ctx->workers->setDefaultKeeper( key );
	return 0;
}
#undef FUNCLASS


