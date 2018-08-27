#pragma once

#include <cpblock.hpp>

namespace usrtos {

template <typename T, typename Compare, size_t AT, size_t Hsize>
class Heap {

public:
	struct _heap_meta { 
		size_t size;
		interprocess_mutex heap_mutex;
	};
	
	struct _shared : _heap_meta {
		size_t heap[Hsize];
	};

private:
	struct _shared *m_pa;
	CPBlock *m_mem;
	std::string name;

	bool less(size_t a, size_t b) {
		typename Compare::less l;
		return l(m_mem->Off2LP<T>(a),m_mem->Off2LP<T>(b));
	};
	long long key(size_t a) {
		typename Compare::key k;
		return k(m_mem->Off2LP<T>(a));
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
	};

	size_t insert(size_t a)
	{
		if(m_pa->size < Hsize) {
			scoped_lock<interprocess_mutex> lock(m_pa->heap_mutex);
			m_pa->heap[m_pa->size] = a;
			m_pa->size++;
			up(m_pa->size-1);
			return 0;
		} else
			return Hsize;
	};

	size_t del(size_t a)
	{
		size_t ret = Hsize;
		if(m_pa->size == 0) 
			return ret;

		scoped_lock<interprocess_mutex> lock(m_pa->heap_mutex);
		int i;
		for(i = 0;i < m_pa->size;i++) {
			if(m_pa->heap[i] == a)
				break;
		}
		if(i != m_pa->size) {
			m_pa->size--;
			m_pa->heap[i]=m_pa->heap[m_pa->size];
			down(i);
			ret = 0;
		}
		
		return ret;
	};

	size_t LP2offset(T* p) {
		return m_mem->LP2offset<T>(p);
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
					<< " i: " << key(m_pa->heap[i])
					<< " left: " << key(m_pa->heap[left])
					<< std::endl;
				err++;
			}
			if((right < m_pa->size) 
				&& less(m_pa->heap[right],m_pa->heap[i]) ) {
				
				std::cout 
					<< Compare::_type() 
					<< " error @" << i << "-" << right
					<< " i: " << key(m_pa->heap[i])
					<< " right: " << key(m_pa->heap[right])
					<< std::endl;
				err++;
			}

			if(debug < i) {
				std::cout 
					<< Compare::_type() 
					<< " No " << i << ":"
					<< key(m_pa->heap[i])
					<< std::endl;
			}
		}
		std::cout << std::flush;
		return err;
	};
}; // class Heap
}; // namespace usrtos