#include <cpblock.hpp>
#include "usrtmem.hpp"
#include "usrtheap.hpp"
#include <chrono>

namespace usrtos {

typedef std::chrono::high_resolution_clock::time_point utime_t;
typedef std::chrono::duration<int, std::micro> micro_type;

typedef long long int64;

inline std::ostream& operator<<(std::ostream& os,const utime_t& h)
{
	auto d = h - std::chrono::high_resolution_clock::now();
	micro_type dd = std::chrono::duration_cast<micro_type>(d);
	os << dd.count();
	return os;
}


enum CBMode { none, abss, me, obj, extwait, extmulti };
typedef struct __callback_argv {
  enum CBMode mode;
  int delay;
  int cnt;
  int argv;
  void *pQ;
  CPBlock::GP gp;
} _callback_argv_t;

struct task {
	int64 ID;
	int64 key;
	int64 from;
	int64 to;
	CPBlock::GP argv;
	int64 callback;
	_callback_argv_t callbackargv;
	utime_t noE;
	utime_t noL;
	utime_t valid;
	int state;
	int ref;
	int version;
	CPBlock::GP mem;
	uuid id;
};

class TaskHeap {

	template <typename Task>
	struct Wait {
		static const std::string _type() { return std::string("wait"); };
		struct less {
			bool operator()(Task *a, Task *b) { return (a->noE < b->noE); };
		};
		struct key {
			utime_t operator()(Task *a) { return a->noE; };
		};
	};

	template <typename Task>
	struct Ready {
		static const std::string _type() { return std::string("ready"); };
		struct less {
			bool operator()(Task *a, Task *b) { return (a->noL < b->noL); };
		};
		struct key {
			utime_t operator()(Task *a) { return a->noL; };
		};
	};

public:
	typedef Fifo<0x1000,0x2000> TaskFifo;
	typedef Heap<task,Wait<task>,0x2000,0xa000> WaitHeap;
	typedef Heap<task,Ready<task>,0xa000,0x10000> ReadyHeap;
	WaitHeap *wait;
	ReadyHeap *ready;
	TaskFifo *tm;

	TaskHeap(CPBlock& m) {
		wait = new WaitHeap(m);
		ready = new ReadyHeap(m);
		tm = new TaskFifo(m);
	};

	void dumpTaskTime(struct task& t) {
		std::cout << "noE: " << t.noE << " "
			<< "noL: " << t.noL << " "
			<< "valid: " << t.valid
			<< std::endl;
	};

	utime_t now() {
		return std::chrono::high_resolution_clock::now();
	};

	utime_t after(long long us) {
		return std::chrono::high_resolution_clock::now()
			+ micro_type(us);
	};

	int adjust() {
		struct task *card = tm->newLP<task>();
		card->noE = now();
		int r = 0;
		auto card_off = wait->LP2offset(card);
		if(wait->_insert(card_off) != wait->NullOffset()) {
			size_t p;
			while((p = wait->pop()) != card_off) {
				if(ready->insert(p) == ready->NullOffset()) {
					wait->del(card_off);
					break;
				}
				r++;
			}
		}
		return r;
	};
};
}; // namespace usrtos