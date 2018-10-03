#include "system.hpp"
#include <internalfuncmap.hpp>

namespace usrtos {

	#define VISITOR(code,name,type) capWorkers##name::run,
	CRunFunc SystemFuncMap[(int)FunctionType::capWorkersEnd+1] = {
		nullptr,
		CAPABILITY_SYSTEM_WORKER_OPERATOR(VISITOR)
		nullptr
	};

	bool validInternalFunc(uint64_t code) {
		return (code < (uint64_t)FunctionType::capWorkersEnd);
	};
};