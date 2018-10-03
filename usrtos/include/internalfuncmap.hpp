#include <cstdint>

namespace usrtos {
	typedef int(*CRunFunc)(void *argv);
	extern CRunFunc SystemFuncMap[];
	bool validInternalFunc(uint64_t code);
};