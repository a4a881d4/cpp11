#pragma once
#include <cstdint>

namespace usrtos {
	
	#define CAPABILITY_SYSTEM_WORKER_OPERATOR(visitOp) \
		visitOp( 1,Clearer,0x1LL)  \
		visitOp( 2,DelCapByKey,0x1LL)  \
		visitOp( 3,DumpQueue,0x1LL)  \
		visitOp( 4,DumpThread,0x1LL)  \
		visitOp( 5,Exit,0x1LL)  \
		visitOp( 6,HelloWorld,0x1LL)  \
		visitOp( 7,InitClock,0x1LL)  \
		visitOp( 8,Keeper,0x1LL)  \
		visitOp( 9,ListCap,0x1LL)  \
		visitOp(10,Script,0x1LL)  \
		visitOp(11,SetCapByKey,0x1LL)  \
		visitOp(12,SetCap,0x1LL)  \
		visitOp(13,SetDefaultKeeper,0x1LL)  \
		visitOp(14,SetKeeper,0x1LL)  \
		visitOp(15,SetWorker,0x1LL)  \
		visitOp(16,Start,0x1LL)  \
		visitOp(17,TaskOnly,0x1LL)  \
		visitOp(18,TaskScript,0x1LL)  \
		visitOp(19,UnlockReady,0x1LL)  \
		visitOp(20,UnlockWait,0x1LL)  \
		visitOp(21,UpdateWorkerKey,0x1LL)

	#define VISITOR(code,name,type) namespace capWorkers##name { int run(void*); };	
	CAPABILITY_SYSTEM_WORKER_OPERATOR(VISITOR)
	#undef VISITOR

	namespace capCallBackLunchTask { int run(void*);};
	#define VISITOR(code,name,type) capWorkers##name = code,
	enum class FunctionType : uint64_t {
		capWorkersNull = 0,
		CAPABILITY_SYSTEM_WORKER_OPERATOR(VISITOR)
		capCallBackLunchTask,
		capWorkersEnd
	};
	#undef VISITOR
};
