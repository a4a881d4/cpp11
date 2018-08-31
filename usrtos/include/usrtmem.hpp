#pragma once
#include <cpblock.hpp>
#include <boost/mpl/size_t.hpp>

namespace usrtos {

template<size_t AT>
class Mem {

public:	
	struct bar {
		size_t pos;
		umutex bar_mutex; 
	};
	
private:
	CPBlock *m_mem;
	struct bar *m_bar;

public:
	Mem(CPBlock& m) { 
		m_mem = &m;
		m_bar = static_cast<struct bar*>((void *)((char *)(m_mem->m_head)+AT));
		m_mem->checkMutex(m_bar->bar_mutex,"bar_mutex"); 
	};

	template <typename T>
	void defaultGP(CPBlock::GP& gp, size_t n=1, size_t aligned=1) {
		size_t s = sizeof(T)*n;
		scoped_lock<umutex> lock(m_bar->bar_mutex);
		if(aligned!=1) {
			size_t m = m_bar->pos%aligned;
			if(m!=0)
				m_bar->pos += (aligned-m); 
		}
		gp.offset = m_bar->pos;
		gp.objsize = s;
		m_bar->pos += s;
	};

	template <typename T>
	T* newLP(size_t n=1, size_t aligned=1) {
		CPBlock::GP gp;
		defaultGP<T>(gp,n,aligned);
		return m_mem->Off2LP<T>(gp.offset);;
	};
	
	template <typename T>
	void newGP(CPBlock::GP& gp, size_t n=1, size_t aligned=1) {
		defaultGP<T>(gp,n,aligned);
		gp.id = m_mem->m_uuid;
	};
	
	void dump() {
		std::cout << "pos: " << m_bar->pos << std::endl;
	};

	virtual bool checkType() {
		return m_mem->checkType("MemHeap");
	};
};

template<size_t AT, size_t END>
class Fifo : public Mem<AT> {

public:
	
	struct fifo_pointer : Mem<AT>::bar {
		int sp;
		int rp;
		umutex fifo_mutex; 
	};
	
	typedef boost::mpl::size_t<((END-AT-sizeof(struct fifo_pointer)-16)/sizeof(size_t))> FIFOSIZE;
		
	struct fifo_addons : fifo_pointer {
		size_t buf[FIFOSIZE::value];
	};

private:
	CPBlock *m_mem;
	struct fifo_addons* m_pa;
	int m_rp;
	void inc(int& a) {
		a++;
		a%=FIFOSIZE::value;
	};
	void incsp() {inc(m_pa->sp);};
	void incrp() {inc(m_pa->rp);};
	void push(size_t off) {
		uscoped_lock lock(m_pa->fifo_mutex);
		if(len()==FIFOSIZE::value-1)
			incrp();
		m_pa->buf[m_pa->sp] = off;
		incsp();
	};
public:

	Fifo(CPBlock& m) : Mem<AT>(m){
		m_mem = &m;
		m_pa = static_cast<struct fifo_addons*>((void *)((char *)(m_mem->m_head)+AT));
		m_mem->checkMutex(m_pa->fifo_mutex,"fifo_mutex"); 
	};

	size_t len() {
		return (m_pa->sp-m_pa->rp+FIFOSIZE::value)%FIFOSIZE::value;
	};
	
	void local_reset() {
		m_rp = m_pa->sp;
	};
	
	size_t local_len() {
		return (m_pa->sp-m_rp+FIFOSIZE::value)%FIFOSIZE::value;
	};
	
	void dumpPoint() {
		std::cout 
			<< "  sp:  " << m_pa->sp 
			<< "  rp:  " << m_pa->rp
			<< "  lrp: " << m_rp
			<< "  llen:" << local_len()
			<< "  len: " << len() 
			<< std::endl;
	};
	
	virtual void dump() {
		Mem<AT>::dump();
		std::cout << "Fifo size: " << FIFOSIZE::value << std::endl;
		dumpPoint();
	};
	
	template<typename T>
	size_t push(T& t) {
		return push<T>(&t);
	};

	template<typename T>
	size_t push(T *t) {
		size_t off = m_mem->LP2offset<T>(t);
		if(m_mem->validOffset(off)) {
			push(off);
		}
		return off; 
	};

	template<typename T>
	T* get() {
		scoped_lock<umutex> lock(m_pa->fifo_mutex);
		if(len()==0)
			return nullptr;
		T* r = m_mem->Off2LP<T>(m_pa->buf[m_pa->rp]);
		incrp();
		return r; 
	};

	template<typename T>
	T* local_get() {
		if(local_len()==0)
			return nullptr;
		T* r = m_mem->Off2LP<T>(m_pa->buf[m_rp]);
		inc(m_rp);
		return r; 
	};	
}; //class Fifo
}; // namespace usrtos
