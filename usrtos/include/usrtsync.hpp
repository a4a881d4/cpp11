#include <usrtconfig.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <atomic>

namespace usrtos {

struct UsrtMutex {

#if USRT_MUTEX_DEBUG
	unsigned long m_threadid = 0;
	void getThreadId()
    {
        std::string threadId=boost::lexical_cast<std::string>(boost::this_thread::get_id());
        m_threadid =std::stoul(threadId,nullptr,16);
    };

    void dump() {
    	std::cout << "Locked or last locked by thread: " << m_threadid << std::endl; 
    };

    void delaySomeTime() {
    	std::cout << "try to lock but ";
    	dump();
    };
#else
    void dump() {};
    void delaySomeTime() {
    	for(int i=0;i<256;i++);
    };
#endif // USRT_MUTEX_DEBUG

	std::atomic_flag m_lock;
	bool try_lock() { // return false means get the lock, the lock is true
		return m_lock.test_and_set(); 
	};

	bool lock() { // return if get the lock
		while(m_lock.test_and_set())
			delaySomeTime();
#if USRT_MUTEX_DEBUG
		getThreadId();
#endif
	};

	void unlock() {
		m_lock.clear();
	}; 

};

class UsrtScopedLock {
public:
	UsrtScopedLock(UsrtMutex& mutex) {
		p_mutex = &mutex;
		p_mutex->lock();
	};
	~UsrtScopedLock() {
		if(p_mutex)
			p_mutex->unlock();
	};
private:
	UsrtMutex *p_mutex = nullptr;
};

};