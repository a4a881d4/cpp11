#include <timing/getnow.hpp>
#include <usrttype.hpp>

namespace usrtos {

struct UsrtClock {
	long long cpu;
	long long sys;

	long long cpu_now() {
		return timing::CPUClock().getNow();
	};

	long long sys_now() {
		utime_t a = std::chrono::high_resolution_clock::now();
		return *((long long*)(&a));
	};

	void peek() {
		auto cpu0 = cpu_now();
		sys = sys_now();
		auto cpu1 = cpu_now();
		if(cpu1-cpu0 > 3000)
			peek();
		else {
			cpu = (cpu0+cpu1)/2;
		}
	};

};

}; //namespace usrtos