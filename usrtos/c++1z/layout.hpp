#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <string.h>
#include <iostream>
#include <boost/mpl/size_t.hpp>

#include <cpblock.hpp>

using namespace boost;
using namespace boost::interprocess;

namespace usrtos {
template <typename Addon, typename type_t, size_t pos, size_t MetaSize>
class MemBlock : public CPBlock {
public:
	virtual bool checkHead() {
		return CPBlock::checkHead() 
		&& checkMetaSize<MetaSize>() 
		&& checkType(type_t::_type());
	};
	Addon *get_addons() {
		return (Addon*)((char*)m_head+pos);
	};	
};

struct bar {
	size_t pos;
	boost::interprocess::interprocess_mutex bar_mutex; 
};

struct fifo_pointer : bar {
	int sp;
	int rp;
	// lock when access sp and rp
	boost::interprocess::interprocess_mutex fifo_mutex; 
};

template <size_t Pos, size_t MetaSize>
struct fifo_addons : fifo_pointer {
	typedef mpl::size_t<((MetaSize-Pos-sizeof(struct fifo_pointer)-16)/sizeof(size_t))> FIFOSIZE;
	size_t buf[FIFOSIZE::value];
	void inc(int& a) {
		a++;
		a%=FIFOSIZE::value;
	};
	void incsp() {inc(sp);};
	void incrp() {inc(rp);};
};

template <typename Addon, typename type_t, size_t Pos, size_t MetaSize>
class MemHeap : public MemBlock<Addon, type_t, Pos, MetaSize> {
public:
	
	typedef MemBlock<Addon, type_t, Pos, MetaSize> MemAddons;
	
	virtual void dump() {
		CPBlock::dump();
		std::cout << "pos:" << MemAddons::get_addons()->pos << std::endl;
	};
	
	virtual bool checkLock() {
		return CPBlock::checkMutex(MemAddons::get_addons()->bar_mutex,"bar_mutex") 
		&& CPBlock::checkLock();
	};

	template <typename T>
	T* newLP(size_t n=1, size_t aligned=1) {
		size_t s = sizeof(T)*n;
		auto pa = MemAddons::get_addons();
		
		scoped_lock<interprocess_mutex> lock(pa->bar_mutex);
		if(aligned!=1) {
			size_t m = pa->pos%aligned;
			if(m!=0)
				pa->pos += (aligned-m); 
		}
		auto r = CPBlock::Off2LP<T>(pa->pos);
		pa->pos += s;
		return r;
	};	
};

template <typename Addon, typename type_t, size_t Pos, size_t MetaSize>
class MemFifo : public MemHeap<Addon, type_t, Pos, MetaSize> {
public:
	
	typedef MemBlock<Addon, type_t, Pos, MetaSize> FifoAddons;
	
	int m_rp; 
	
	size_t len() {
		auto pa = FifoAddons::get_addons();
		return (pa->sp-pa->rp+Addon::FIFOSIZE::value)%Addon::FIFOSIZE::value;
	};
	
	void local_reset() {
		m_rp = FifoAddons::get_addons()->sp;
	};
	
	size_t local_len() {
		auto pa = FifoAddons::get_addons();
		return (pa->sp-m_rp+Addon::FIFOSIZE::value)%Addon::FIFOSIZE::value;
	};
	
	void dumpPoint() {
		std::cout 
			<< "  sp:  " << FifoAddons::get_addons()->sp 
			<< "  rp:  " << FifoAddons::get_addons()->rp
			<< "  lrp: " << m_rp
			<< "  llen:" << local_len()
			<< "  len: " << len() 
			<< std::endl;
	};
	
	virtual void dump() {
		MemHeap<Addon, type_t, Pos, MetaSize>::dump();
		dumpPoint();
	};
	
	virtual bool checkLock() {
		return CPBlock::checkMutex(FifoAddons::get_addons()->fifo_mutex,"fifo_mutex")
		&& MemHeap<Addon, type_t, Pos, MetaSize>::checkLock();
	};

	template<typename T>
	size_t push(T& t) {
		auto pa = FifoAddons::get_addons();
		auto off = CPBlock::LP2offset<T>(&t);
		if(CPBlock::validOffset(off)) {
			scoped_lock<interprocess_mutex> lock(pa->fifo_mutex);
			if(len()==Addon::FIFOSIZE::value-1)
				pa->incrp();
			pa->buf[pa->sp] = off;
			pa->incsp();
		}
		return off; 
	};

	template<typename T>
	T* get() {
		auto pa = FifoAddons::get_addons();
		scoped_lock<interprocess_mutex> lock(pa->fifo_mutex);
		if(len()==0)
			return nullptr;
		T* r = CPBlock::Off2LP<T>(pa->buf[pa->rp]);
		pa->incrp();
		return r; 
	};

	template<typename T>
	T* local_get() {
		auto pa = FifoAddons::get_addons();
		if(local_len()==0)
			return nullptr;
		T* r = CPBlock::Off2LP<T>(pa->buf[m_rp]);
		pa->inc(m_rp);
		return r; 
	};
};

class MemHeapType {
	public:
	static const char * _type() { return "MemHeap"; };
};

class BlockType {
	public:
	static const char * _type() { return "MemBlock"; };
};

class FifoType {
	public:
	static const char * _type() { return "MemFifo"; };
};

class MemoryBlockLayout {
	
public:
	typedef MemHeap<struct bar, MemHeapType, 0x200, 0x1000> UsrtMem;
	typedef MemHeap<struct bar, BlockType, 0x200, 0x1000> UsrtBlk;
	typedef MemFifo<struct fifo_addons<0x200, 0x1000>, FifoType, 0x200, 0x1000> UsrtFifo;

};
}; // namespace usrtos
