#include <cpblock.hpp>
#include "usrtmem.hpp"
#include "usrtheap.hpp"

namespace usrtos {

typedef long long utime_t;
typedef long long int64;

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

template <typename Task>
struct Wait {
	static const std::string _type() { return std::string("wait"); };
	struct _less {
		bool operator()(Task *a, Task *b) { return (a->noE < b->noE); };
	};
	struct _key {
		utime_t operator()(Task *a) { return a->noE; };
	};
	typedef struct _less less;
	typedef struct _key key;

};

template <typename Task>
struct Ready {
	static const std::string _type() { return std::string("ready"); };
	struct _less {
		bool operator()(Task *a, Task *b) { return (a->noL < b->noL); };
	};
	struct _key {
		utime_t operator()(Task *a) { return a->noL; };
	};
	typedef struct _less less;
	typedef struct _key key;

};

class TaskHeap {

public:
	typedef Fifo<0x200,0x400> TaskFifo;
	typedef Heap<task,Wait<task>,0x400,0x80> WaitHeap;
	typedef Heap<task,Ready<task>,0xa00,0x80> ReadyHeap;
	WaitHeap *wait;
	ReadyHeap *ready;
	TaskFifo *tm;

	TaskHeap(CPBlock& m) {
		wait = new WaitHeap(m);
		ready = new ReadyHeap(m);
		tm = new TaskFifo(m);
	};




};
}; // namespace usrtos