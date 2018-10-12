#include "system.hpp"
#include <internalfuncmap.hpp>
#include <string>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>
namespace usrtos {

	#define VISITOR(code,name,type) capWorkers##name::run,
	CRunFunc SystemFuncMap[(int)InternelFunctionType::capWorkersEnd+1] = {
		nullptr,
		CAPABILITY_SYSTEM_WORKER_OPERATOR(VISITOR)
		nullptr
	};
	#undef VISITOR

	bool validInternalFunc(uint64_t code) {
		return (code < (uint64_t)InternelFunctionType::capWorkersEnd);
	};

	struct InternalFunctionItem {
		int code;
		char name[32];
	};

	#define VISITOR(code,name,type) {code,BOOST_PP_STRINGIZE(BOOST_PP_CAT(capWorkers,name))},
	InternalFunctionItem InternalFunctions[(int)InternelFunctionType::capWorkersEnd+1] = {
		{0,"Null"},
		CAPABILITY_SYSTEM_WORKER_OPERATOR(VISITOR)
		{(int)InternelFunctionType::capCallBackLunchTask,"capCallBackLunchTask"},
		{(int)InternelFunctionType::capWorkersEnd,"End"}
	};
	#undef VISITOR

	int name2internalFunc(std::string name) {
		for(int i=0;i<(int)InternelFunctionType::capWorkersEnd;i++) {
			if(std::string(InternalFunctions[i].name) == name)
				return i;
		}
		return 0;
	};

	std::string internalFuncName(int i) {
		return std::string(InternalFunctions[i].name);
	};
};