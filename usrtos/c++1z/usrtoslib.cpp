#include <loglevel.hpp>
#include <timing/clock.hpp>

namespace usrtos {	

	LogLevel	SYSLOG;
	LogLevel	 DEBUG;
	LogLevel	  INFO;
	LogLevel	  WARN;
	LogLevel	 ERROR;
	LogLevel	 FATAL;

	namespace timing {
		CPU2SYS * CPUClock::c2s = nullptr;
		CPU2SYS * SYSClock::c2s = nullptr;
	};
};
