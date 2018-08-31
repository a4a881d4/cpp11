#pragma once
#include <usrttype.hpp>
#include <cpblock.hpp>
#include <boost/mpl/size_t.hpp>

namespace usrtos {

template <typename T, typename PointerType, typename Compare, size_t AT, size_t END>
class Heap {

public:
	struct _heap_meta { 
		size_t size;
		umutex heap_mutex;
	};

	typedef boost::mpl::size_t<((END-AT-sizeof(struct _heap_meta)-16)/sizeof(PointerType))> Hsize;
	
	struct _shared : _heap_meta {
		PointerType heap[Hsize::value];
	};

private:
	struct _shared *m_pa;
	CPBlock *m_mem;
	std::string name;
	typename Compare::key m_key;
	typename Compare::less m_less;

	bool less(PointerType a, PointerType b) {
		return m_less(a,b);
	};

	auto KEY(PointerType a) {
		 return m_key(a);
	};

	void down(size_t index)
	{
		if(m_pa->size <= index)
			return;
		
		size_t left = index*2+1;
		size_t right = index*2+2;
		size_t small = index;
		
		if((left < m_pa->size) 
			&& less(m_pa->heap[left],m_pa->heap[small]))
				small=left;
		
		if((right < m_pa->size) 
			&& less(m_pa->heap[right],m_pa->heap[small]))
				small=right;
		
		if(small == index)
			return;
		
		auto a = m_pa->heap[index];
		m_pa->heap[index] = m_pa->heap[small];
		m_pa->heap[small] = a;
		
		down(small);
	};

	void up(size_t index)
	{
		if(index == 0)
			return;

		size_t _up = (index-1)/2;
		if(less(m_pa->heap[index],m_pa->heap[_up])) {
			auto a = m_pa->heap[index];
			m_pa->heap[index] = m_pa->heap[_up];
			m_pa->heap[_up] = a;
			up(_up);
		}
	};

public:
	Heap(CPBlock& m) {
		m_mem = &m;
		m_pa = static_cast<struct _shared*>((void *)((char *)(m_mem->m_head)+AT));
		name = Compare::_type();
		m_mem->checkMutex(m_pa->heap_mutex,"heap_mutex"+name); 
		m_less.set(&m);
		m_key.set(&m);
	};

	static const size_t HeapSize() { return Hsize::value; };
	CPBlock *getMem() { return m_mem; };

	PointerType _insert(PointerType a)
	{
		if(m_pa->size < Hsize::value) {
			uscoped_lock lock(m_pa->heap_mutex);
			m_pa->heap[m_pa->size] = a;
			m_pa->size++;
			up(m_pa->size-1);
			return a;
		} else
			return PointerType::Null();
	};
	PointerType insert(PointerType a)
	{
		if(m_pa->size < Hsize::value-1) {
			return _insert(a);
		} else
			return PointerType::Null();
	};
	PointerType del(PointerType a)
	{
		PointerType ret = PointerType::Null();
		if(m_pa->size == 0) 
			return ret;

		uscoped_lock lock(m_pa->heap_mutex);
		int i;
		for(i = 0;i < m_pa->size;i++) {
			if(a == m_pa->heap[i])
				break;
		}
		if(i != m_pa->size) {
			m_pa->size--;
			m_pa->heap[i]=m_pa->heap[m_pa->size];
			down(i);
			ret = a;
		}
		
		return ret;
	};

	int check(int debug = 0) {
		int err = 0;
		int i;
		for(i = 0;i < m_pa->size;i++) {
			size_t left = i*2+1;
			size_t right = i*2+2;
			if((left < m_pa->size) 
				&& less(m_pa->heap[left],m_pa->heap[i])) {
				
				std::cout
					<< Compare::_type() 
					<< " error @" << i << "-" << left
					<< " i: " << KEY(m_pa->heap[i])
					<< " left: " << KEY(m_pa->heap[left])
					<< std::endl;
				err++;
			}
			if((right < m_pa->size) 
				&& less(m_pa->heap[right],m_pa->heap[i]) ) {
				
				std::cout 
					<< Compare::_type() 
					<< " error @" << i << "-" << right
					<< " i: " << KEY(m_pa->heap[i])
					<< " right: " << KEY(m_pa->heap[right])
					<< std::endl;
				err++;
			}

			if(debug < i) {
				std::cout 
					<< Compare::_type() 
					<< " No " << i << ":"
					<< KEY(m_pa->heap[i])
					<< std::endl;
			}
		}
		std::cout << std::flush;
		return err;
	};

	void dumpHeap() {
		int i;
		std::cout << "Heap size: " << m_pa->size << std::endl;
		for( int i = 0;i < m_pa->size;i++ ) {
			std::cout << "No " << i << ": "
				<< KEY(m_pa->heap[i])
				<< std::endl;
		}
	};

	PointerType pop() {
		auto p = m_pa->heap[0];
		if(m_pa->size > 0) {
			del(p);
			return p;
		}
		else
			return PointerType::Null();
	};
}; // class Heap
}; // namespace usrtos