#define FUNCLASS capWorkersScript
#include <capabilityAPI.hpp>
#include <usrtworker.hpp>
#include <vm/opcodes.hpp>
using namespace usrtos;

static struct vm::VMContext *vmCtx = nullptr;
static struct vm::JITVisitor *visitor = nullptr;
int FUNCLASS::run( void *argv ) {
	struct mainWorkerCTX *ctx = (struct mainWorkerCTX *)argv;
	if(!vmCtx) {
		vmCtx = new struct vm::VMContext(*(ctx->workers));
		visitor = new struct vm::JITVisitor(*vmCtx);
	}
	
	vm::OperatorStream os(static_cast<U8*>(ctx->argv),ctx->len);
	vm::Decode d;
	d.run(*visitor,os,vm::RunMode::run);
	return 1;
}


