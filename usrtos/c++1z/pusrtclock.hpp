#include <timing/clock.hpp>
#include <usrttype.hpp>

namespace usrtos {

timing::CPU2SYS c2s;

struct UsrtClock {
	long long cpu;
	long long sys;

	long long cpu_now() {
		return timing::CPUClock().now();
	};

	long long sys_now() {
		auto a = timing::SYSClock().now();
		return a;
	};

	void peek_i(int i=0) {
		auto cpu0 = cpu_now();
		sys = sys_now();
		auto cpu1 = cpu_now();
		if(cpu1-cpu0 > 500 && i < 10)
			peek_i(i+1);
		else {
			cpu = (cpu0+cpu1)/2;
		}
	};
	void peek() {
		peek_i(0);
	};

	double update() {
		if(!timing::CPUClock::c2s)
			timing::CPUClock::c2s = &c2s;
		timing::CPUClock::c2s->update();
		return timing::CPUClock::c2s->err;
	};
};
}; //namespace usrtos