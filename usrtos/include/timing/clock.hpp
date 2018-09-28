#pragma once
#include <chrono>
#include <sstream>
#include <ratio>

namespace usrtos { namespace timing {
	using namespace std::chrono;

	typedef long long int time_t;
	typedef high_resolution_clock::time_point systime_t;
	typedef duration<int, std::micro> micro_type;
	typedef duration<int, std::nano> ns;
	typedef duration<double, std::ratio<1,1> > Second;
	inline std::ostream& operator<<(std::ostream& os,const systime_t& h)
	{
		auto d = h - high_resolution_clock::now();
		micro_type dd = duration_cast<micro_type>(d);
		os << dd.count();
		return os;
	}
	
	systime_t getSysNow() {
		return high_resolution_clock::now();
	};
	
	system_clock::time_point getWallNow() {
		return system_clock::now();
	};

	time_t getCpuNow() {
		typedef struct { unsigned long t[2]; } _timing;
		#define timing_now(x) asm volatile(".byte 15;.byte 49" : "=a"((x)->t[0]),"=d"((x)->t[1]))
		_timing now;
		timing_now(&now);
		return (time_t)now.t[0]+4294967296LL*(time_t)now.t[1];
	};

	struct CPU2SYS {
		time_t C0,S0,W0;
		time_t lastC,lastS;
		time_t nowC,nowS;
		time_t rC,rS,rW;
		double v;
		double err;
		bool updateV;
		operator bool() {
			return !updateV;
		}
		void peek_i(int i=0) {
			time_t cpu0 = getCpuNow();
			auto a = getSysNow();
			time_t sys0 = *(time_t*)(&a);
			auto b = getWallNow().time_since_epoch();
			auto sb = duration_cast<Second>(b);
			time_t wall = sb.count()*1e9;
			auto c = getSysNow();
			time_t sys1 = *(time_t*)(&c);
			time_t cpu1 = getCpuNow();
			if( cpu1-cpu0 > 2000 && i<10 ) {
				peek_i(i+1);
			} else {
				rC = (cpu0+cpu1)/2;
				rS = (sys0+sys1)/2;
				rW = wall;
				if(i >= 10)
					std::cout << " time diff " << cpu1-cpu0 << std::endl;
			}
		};
		void peek() {
			peek_i(0);
		};
		void reset() {
			C0 = 0;
			S0 = 0;
		};
		void init() {
			C0 = 0;
			S0 = 0;
			updateV = true;
		};
		void update() {
			peek();
			if(C0 == 0) {
				C0 = rC;
				S0 = rS;
				W0 = rW - S0;
				lastC = 0;
				lastS = 0;
			} else {
				nowC = rC - C0;
				nowS = rS - S0;
				if(lastC == 0 && updateV) {
					v = ((double)nowS+0.5)/((double)nowC+0.5);
					lastS = nowS;
					updateV = false;
				} else {
					double dC = nowC - lastC;
					double est = dC*v + (double)lastS;
					err = (double)nowS - est;
					if(fabs(err) > 500) {
						reset();
						if(fabs(err) > 5000)
							init();
					} else {
						v += 0.5*err/dC;
						lastS = (time_t)est;
					}
				}
				lastC = nowC;
			}
		};

		time_t toSys(time_t cpu) {
			if(!updateV) {
				time_t dc = cpu-C0-lastC;
				double ds = (double)dc * v;
				return (time_t)ds+S0+lastS+W0;
			} else {
				return 0;
			}
		};

		time_t toCpu(time_t sys) {
			if(!updateV) {
				time_t ds = sys-S0-lastS-W0;
				double dc = (double)ds / v;
				return (time_t)dc+C0+lastC;
			} else {
				return 0;
			}
		};
		time_t mulCpu(time_t sys) {
			if(!updateV) {
				double cpu = (double)sys / v;
				return (time_t)cpu;
			} else {
				return sys;
			}
		};
		void dump(std::stringstream& s) {
			s << " v: " << v;
			s << " e: " << err;
			auto a = getSysNow();
			time_t b = *(time_t*)(&a)+W0;
			s << " now: " << getCpuNow() - toCpu(b);
			s << std::endl;
		};
		CPU2SYS() {
			init();
		};
	};
	struct CPUClock {
		typedef time_t utime_t;
		static CPU2SYS *c2s;
		time_t now() const {
			return getCpuNow();
		};
		time_t after(time_t t) const {
			return getCpuNow() + t;
		};
		systime_t systime() const {
			time_t n = now();
			if(c2s) {
				if(*c2s){
					time_t s = c2s->toSys(n) - c2s->W0;
					return *(systime_t*)s;
				}
			} 
			return getSysNow();
		};

		time_t fromns(time_t ns) {
			if(c2s) {
				if(*c2s) {
					return (time_t)((double)ns/c2s->v);	
				} 
			}
			return ns;
		};

		time_t micro_type(time_t ns) {
			return fromns(ns);
		};
	};

	struct SYSClock {
		typedef systime_t utime_t;
		static CPU2SYS *c2s;
		time_t now() const {
			auto a = getSysNow();
			return *(time_t *)(&a);
		};

		time_t after(time_t t) const {
			auto a = getSysNow() + micro_type(t);
			return *(time_t *)(&a);
		};
		
		systime_t systime() const {
			return getSysNow();
		};

		time_t fromns(time_t ns) {
			return ns;
		};

		time_t wall_time() {
			if(c2s) {
				if(*c2s) {
					return now()+c2s->W0;
				}
			}
			return now();
		};
	};

	CPU2SYS * CPUClock::c2s = nullptr;
	CPU2SYS * SYSClock::c2s = nullptr;
}}