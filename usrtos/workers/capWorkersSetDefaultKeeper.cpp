#define FUNCLASS capWorkersSetDefaultKeeper
#include <capabilityAPI.hpp>
#include <usrtworker.hpp>

using namespace usrtos;

int FUNCLASS::run( void *argv ) {
	
	struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
	if( !ctx->workers )
		return -1;
	struct CapabilityMeta keeper_meta;
	memset(&keeper_meta,0,sizeof(struct CapabilityMeta));
	strncpy((char *)keeper_meta.name,"capWorkersKeeper",32);
	strncpy((char *)keeper_meta.type,"USRTOS",32);
	keeper_meta.version = sha1str(USRTOSVERSION);
	uuid key = meta2uuid(keeper_meta);

	ctx->workers->setDefaultKeeper( key );
	return 0;
}



