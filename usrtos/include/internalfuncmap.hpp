#include <cstdint>
#include <string>

namespace usrtos {
	typedef int(*CRunFunc)(void *argv);
	extern CRunFunc SystemFuncMap[];
	bool validInternalFunc(uint64_t code);
	int name2internalFunc(std::string name);
	std::string internalFuncName(int i);
};