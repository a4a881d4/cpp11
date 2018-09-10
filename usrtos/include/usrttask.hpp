#include <usrtkey.hpp>
#include <cpblock.hpp>
#include <usrtmem.hpp>
#include <usrtheap.hpp>
#include <task.hpp>

#include <chrono>

namespace usrtos {




class OffsetPtr {
public:
	size_t p;
	OffsetPtr() : p(0) {};
	OffsetPtr(size_t o) : p(o){};
	OffsetPtr(OffsetPtr& a) { p = a.p; };
	
	static const OffsetPtr Null() { return OffsetPtr(0xffffffffffffffff); };
	
	template<typename T>
	size_t LP2offset(T* tp, CPBlock *mem) {
		p = mem->LP2offset<T>(tp);
		return p;
	};

	template<typename T>
	T* Off2LP(CPBlock *mem) {
		return mem->Off2LP<T>(p);
	};

};

inline bool operator==(OffsetPtr a, OffsetPtr b) { return (a.p == b.p); };
inline bool operator!=(OffsetPtr a, OffsetPtr b) { return (a.p != b.p); };

class TaskHeap {

	template <typename Task>
	struct Wait {
		static const std::string _type() { return std::string("wait"); };
		struct less {
			CPBlock *mem;
			void set(CPBlock *m) { mem = m; };
			bool operator()(OffsetPtr a, OffsetPtr b) { 
				return (a.Off2LP<task>(mem)->noE < b.Off2LP<task>(mem)->noE); 
			};
		};
		struct key {
			CPBlock *mem;
			void set(CPBlock *m) { mem = m; };
			utime_t operator()(OffsetPtr a) {
				return a.Off2LP<task>(mem)->noE; 
			};
		};
	};

	template <typename Task>
	struct Ready {
		static const std::string _type() { return std::string("ready"); };
		struct less {
			CPBlock *mem;
			void set(CPBlock *m) { mem = m; };
			bool operator()(OffsetPtr a, OffsetPtr b) { 
				return (a.Off2LP<task>(mem)->noL < b.Off2LP<task>(mem)->noL); 
			};
		};
		struct key {
			CPBlock *mem;
			void set(CPBlock *m) { mem = m; };
			utime_t operator()(OffsetPtr a) { 
				return a.Off2LP<task>(mem)->noL; 
			};
		};
	};
	bool checkCard() {
		int64 * c = (int64*)card;
		if((c[0]|c[1]|c[2]|c[3]) != 0) {
			card = tm->newLP<task>();
			memset(card,0,sizeof(task));
			return false;
		}
		return true;
	}
public:
	typedef Fifo<0x1000,0x2000> TaskFifo;
	typedef Heap<task,OffsetPtr,Wait<task>,0x2000,0xa000> WaitHeap;
	typedef Heap<task,OffsetPtr,Ready<task>,0xa000,0x10000> ReadyHeap;
	WaitHeap *wait;
	ReadyHeap *ready;
	TaskFifo *tm;
	struct task *card;

	TaskHeap(CPBlock& m) {
		wait = new WaitHeap(m);
		ready = new ReadyHeap(m);
		tm = new TaskFifo(m);
		card = tm->newLP<task>();
		memset(card,0,sizeof(task));
	};
	

	void dumpTask(struct task& t) {
		std::cout << UsrtKey::key2string(t.key) << std::endl;
		dumpTaskTime(t);
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

	int update() {
		if(!checkCard())
			std::cout << "new card" << std::endl;

		card->noE = now();
		int r = 0;
		
		OffsetPtr card_off;
		card_off.LP2offset(card,wait->getMem());

		if(wait->_insert(card_off) != OffsetPtr::Null()) {
			OffsetPtr p;
			while((p = wait->pop()) != card_off) {
				if(ready->insert(p) == OffsetPtr::Null()) {
					wait->del(card_off);
					break;
				}
				r++;
			}
		}
		return r;
	};

	void dumpHeap() {
		std::cout << "Wait Heap" << std::endl;
		wait->dumpHeap();
		size_t s;
		OffsetPtr *h = wait->getHeap(s);
		for(int i = 0;i < s;i++) {
			OffsetPtr p = h[i];
			struct task * t = p.Off2LP<task>(wait->getMem());
			dumpTask(*t);
		}
		std::cout << "Ready Heap" << std::endl;
		ready->dumpHeap();
		h = ready->getHeap(s);
		for(int i = 0;i < s;i++) {
			OffsetPtr p = h[i];
			struct task * t = p.Off2LP<task>(ready->getMem());
			dumpTask(*t);
		}
	};

	task* pop() {
        auto offp = ready->pop();
        if(offp != OffsetPtr::Null())
        	return offp.Off2LP<task>(ready->getMem());
        else
        	return nullptr;
    };

    bool insert(task *t) {
    	OffsetPtr t_off;
    	if(t_off.LP2offset(t,wait->getMem()) != wait->getMem()->getHead()->dataSize)
    		if(wait->insert(t_off) != OffsetPtr::Null())
    			return true;
    		else
    			return false;
    	else 
    		return false;
    };
};
}; // namespace usrtos