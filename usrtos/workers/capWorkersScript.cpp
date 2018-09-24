#define FUNCLASS capWorkersScript
#include <capabilityAPI.hpp>
#include <usrtworker.hpp>
#include <vm/opcodes.hpp>
using namespace usrtos;

int FUNCLASS::run( void *argv ) {
	struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
	struct vm::VMContext vmCtx(*(ctx->workers));
	struct vm::JITVisitor visitor(vmCtx);

	vm::OperatorStream os(static_cast<U8*>(ctx->argv),256);
	vm::Decode d;
	d.run(visitor,os,true);
	return 1;
}


