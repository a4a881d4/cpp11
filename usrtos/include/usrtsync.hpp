#include <usrtconfig.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <atomic>

namespace usrtos {

struct UsrtMutex {

#if USRT_MUTEX_DEBUG
	unsigned long m_threadid = 0;
	char sf[64];
	int ln;
	void getThreadId()
    {
        std::string threadId=boost::lexical_cast<std::string>(boost::this_thread::get_id());
        m_threadid =std::stoul(threadId,nullptr,16);
    };

    void dump() {
    	sf[63] = '\0';
    	std::cout << "Locked or last locked by thread: " << "@" << ln << "@" << sf << " " << m_threadid << std::endl; 
    };

    void delaySomeTime() {
    	std::cout << "try to lock but ";
    	dump();
    };

    int mutex_ncpy(const char* c, size_t cnt) {
    	char *p = (char *)c;
    	int cc = cnt;
        char *mark = nullptr;
    	while(*p!='\0' && cc>0) {
            if(*p=='/')
                mark = p;
    		cc--;
    		++p;
    	}
    	if(mark) 
    		p = mark+1;
    	else 
    		p=(char *)c;
    	char *r = sf;
    	cc = cnt;
    	while(*p!='\0' && cc>0) {
    		*r = *p;
    		++r;
    		++p;
    		cc--;
    	}
        if(cc>0) *r = '\0';
    	return cnt - cc;
    }
#else
    void dump() {};
    void delaySomeTime() {
    	for(int i=0;i<256;i++);
    };
#endif // USRT_MUTEX_DEBUG

	std::atomic_flag m_lock;
	bool try_lock() { // return true means get the lock, the lock is true
		return !m_lock.test_and_set(); 
	};

	bool lock() { // return if get the lock
		while(m_lock.test_and_set())
			delaySomeTime();
#if USRT_MUTEX_DEBUG
		getThreadId();
#endif
        return true;
	};

	void unlock() {
		m_lock.clear();
	}; 

};

class UsrtScopedLock {
public:
#if USRT_MUTEX_DEBUG	
	UsrtScopedLock(UsrtMutex& mutex, const char *file=__FILE__, int lineno=__LINE__) {
		mutex.mutex_ncpy(file,63);
		mutex.ln = lineno;
#else
	UsrtScopedLock(UsrtMutex& mutex) {
#endif		
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