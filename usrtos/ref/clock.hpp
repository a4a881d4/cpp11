#pragma once
#include <chrono>
#include <sstream>

namespace usrtos { namespace timing {
	using namespace std::chrono;

	typedef long long int time_t;
	
	typedef duration<int, std::nano> nano_type;
	typedef time_point<system_clock,nano_type> systime_t;
	
	time_t sys2time(systime_t a) {
		nano_type ns = duration_cast<nano_type>(a.time_since_epoch());
		return ns.count();
	}
	
	inline std::ostream& operator<<(std::ostream& os,const systime_t& h)
	{
		auto d = h - system_clock::now();
		nano_type dd = duration_cast<nano_type>(d);
		os << dd.count();
		return os;
	}
	
	systime_t getSysNow() {
		return system_clock::now();  //time_from_eproch()
	};
	
	time_t getCpuNow() {
		typedef struct { unsigned long t[2]; } _timing;
		#define timing_now(x) asm volatile(".byte 15;.byte 49" : "=a"((x)->t[0]),"=d"((x)->t[1]))
		_timing now;
		timing_now(&now);
		return (time_t)now.t[0]+4294967296LL*(time_t)now.t[1];
	};

	struct CPU2SYS {
		time_t C0,S0;
		time_t lastC,lastS;
		time_t nowC,nowS;
		time_t rC,rS;
		double v;
		double err;
		bool updateV;
		operator bool() {
			return !updateV;
		}
		void peek_i(int i=0) {
			time_t cpu0 = getCpuNow();
			auto a = getSysNow();
			time_t sys0 = sys2time(a);
			time_t cpu1 = getCpuNow();
			if( cpu1-cpu0 > 500 || i>10 ) {
				peek_i(i+1);
			} else {
				rC = (cpu0+cpu1)/2;
				rS = sys0;
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
				return (time_t)ds+S0+lastS;
			} else {
				return 0;
			}
		};

		time_t toCpu(time_t sys) {
			if(!updateV) {
				time_t ds = sys-S0-lastS;
				double dc = (double)ds / v;
				return (time_t)dc+C0+lastC;
			} else {
				return 0;
			}
		};
		void dump(std::stringstream& s) {
			s << " v: " << v;
			s << " e: " << err;
			auto a = getSysNow();
			time_t b = sys2time(a);
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
				time_t s = c2s->toSys(n);
				systime_t a;
				a += nano_type(s);
				return a;
			} else {
				return getSysNow();
			}
		};
		time_t fromns(time_t ns) {
			if(c2s) {
				if(*c2s) {
					return (time_t)((double)ns/c2s->v);	
				} 
			}
			return ns;
		};
	};

	struct SYSClock {
		typedef systime_t utime_t;
		static CPU2SYS *c2s;
		time_t now() const {
			auto a = getSysNow();
			return sys2time(a);
		};

		time_t after(time_t t) const {
			systime_t a = getSysNow() + nano_type(t);
			return sys2time(a);
		};
		
		systime_t systime() const {
			return getSysNow();
		};
		time_t fromns(time_t ns) {
			return ns;
		};
	};

	CPU2SYS * CPUClock::c2s = nullptr;
	CPU2SYS * SYSClock::c2s = nullptr;
}}