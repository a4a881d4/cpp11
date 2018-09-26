#pragma once

#include <usrtconfig.hpp>
#include <chrono>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <usrtsync.hpp> 
#include <usrtkey.hpp>
#include <timing/clock.hpp>

namespace usrtos {

typedef timing::time_t utime_t;
typedef timing::CPUClock utime;

typedef long long int64;

// typedef boost::interprocess::interprocess_mutex umutex;
// typedef boost::interprocess::scoped_lock<umutex> uscoped_lock;

typedef struct UsrtMutex umutex;
typedef UsrtScopedLock uscoped_lock;


#if USRT_MUTEX_DEBUG
#define USRT_SCOPED_LOCK(x) uscoped_lock lock_internal(x,__FILE__,__LINE__)
#else
#define USRT_SCOPED_LOCK(x) uscoped_lock lock_internal(x)
#endif

};
